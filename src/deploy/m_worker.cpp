/*
 * m_worker.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_worker.h"

Worker::Worker() {

}

Worker::~Worker() {

}

void Worker::init() {
	/* get the local host ip. */
	char hostname[128];
	struct hostent *hent;

	gethostname(hostname, sizeof(hostname));
	hent=gethostbyname(hostname);

	Logging::getInstance()->getInstance()->log(trace, "[trace]: get local host ip address.");

	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);

	Sender *sender=new Sender(COORDINATOR_THERON+1000);
	sender->m_connect("127.0.0.1");//master
	sender->m_send(ip);

	stringstream ip_port;
	ip_port<<"tcp://"<<string(ip)<<":"<<WORKER_THERON;

	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	/* todo: a id manager to give a id to worker. */
	Theron::EndPoint *endpoint=new Theron::EndPoint("worker", ip_port.str().c_str());
	Theron::Framework *framework=new Theron::Framework(*endpoint);

	acn_=new AConnection(endpoint, *framework, "worker_actor");
	acn_->send();

}
