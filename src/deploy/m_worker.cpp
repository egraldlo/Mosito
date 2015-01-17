/*
 * m_worker.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_worker.h"

void Worker::init() {
	/* get the local host ip. */
	char hostname[128];
	struct hostent *hent;

	gethostname(hostname, sizeof(hostname));
	hent=gethostbyname(hostname);

	Logging::getInstance()->getInstance()->log(trace, "get local host ip address.");

	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);

	stringstream ip_port_worker,ip_port_master;
	int theron_worker_port=Configuration::getInstance()->get_theron_worker_port();
	ip_port_worker<<"tcp://"<<string(ip)<<":"<<theron_worker_port;

	/* todo: solve the master ip problem. */
	string master_ip=Configuration::getInstance()->get_coordinator_ip();
	ip_port_master<<"tcp://"<<master_ip<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, "my ip and port is: ");
	Logging::getInstance()->getInstance()->log(trace, ip_port_worker.str().c_str());

#ifdef SINGLE_NODE_TEST
	/* for test on a single node. */
	stringstream worker_endpoint;
	worker_endpoint<<"worker_"<<theron_worker_port;
	Theron::EndPoint endpoint(worker_endpoint.str().c_str(), ip_port_worker.str().c_str());
	endpoint.Connect(ip_port_master.str().c_str());
#endif

#ifndef SINGLE_NODE_TEST
	Theron::EndPoint endpoint("worker", ip_port_worker.str().c_str());
	endpoint.Connect(ip_port_master.str().c_str());
#endif

	endpoint_= &endpoint;
//	endpoint_=new Theron::EndPoint("worker", ip_port_worker.str().c_str());
//	endpoint_->Connect(ip_port_master.str().c_str());

	Sender *sender=new Sender(COORDINATOR_THERON+1000);
	sender->m_connect(master_ip.c_str());//master
	sender->m_send(ip, 32);
	sender->m_close();

	printf("hello sending ---------------->\n");

	Theron::Framework framework(*endpoint_);
//	Theron::Framework framework(endpoint);

	framework.Send(MessageT("hello"), Theron::Address(), Theron::Address("register"));

	executor_m_=ExecutorMaster::getInstance(&endpoint);
	executor_m_->init_executor();

	executor_s_=new ExecutorSlave(&endpoint);
	executor_s_->init_executor();

}
