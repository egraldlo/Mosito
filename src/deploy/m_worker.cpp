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
	ip_port_worker<<"tcp://"<<string(ip)<<":"<<WORKER_THERON;
	/* todo: solve the master ip problem. */
	ip_port_master<<"tcp://"<<string("127.0.0.1")<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, "my ip and port is: ");
	Logging::getInstance()->getInstance()->log(trace, ip_port_worker.str().c_str());

	Theron::EndPoint endpoint("worker", ip_port_worker.str().c_str());
	endpoint.Connect(ip_port_master.str().c_str());

	endpoint_= &endpoint;
//	endpoint_=new Theron::EndPoint("worker", ip_port_worker.str().c_str());
//	endpoint_->Connect(ip_port_master.str().c_str());

	Sender *sender=new Sender(COORDINATOR_THERON+1000);
	sender->m_connect("127.0.0.1");//master
	sender->m_send(ip);
	sender->m_close();

	printf("hello sending ---------------->\n");

	Theron::Framework framework(*endpoint_);
//	Theron::Framework framework(endpoint);

	framework.Send(MessageT("hello"), Theron::Address(), Theron::Address("register"));

	executor_s_=new ExecutorSlave(endpoint_);
}
