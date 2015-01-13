/*
 * m_coordinator.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_coordinator.h"

void Coordinator::init() {
	/* get the local host ip. */
	char hostname[128];
	struct hostent *hent;

	gethostname(hostname, sizeof(hostname));
	hent=gethostbyname(hostname);

	Logging::getInstance()->getInstance()->log(trace, "get local host ip address.");

	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);

	stringstream ip_port;
	ip_port<<"tcp://"<<string(ip)<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, "my ip and port is: ");
	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	endpoint_=new Theron::EndPoint("coordinator", ip_port.str().c_str());
	pthread_t register_table;
	/* todo: complete the function. */
	if(pthread_create(&register_table, 0, register_worker, this)==0) {
		Logging::getInstance()->getInstance()->log(trace, "register worker thread is running.");
	}

	aconnect(ip_port.str().c_str());

	Theron::Receiver receiver(*endpoint_, "coordinator_receiver");
	Theron::Framework framework(*endpoint_);

	acn_= new Thandler(framework, "register");

	receiver.Wait();

	executor_m_=new ExecutorMaster(endpoint_);

}

void* Coordinator::register_worker(void *args) {
	Coordinator *pthis=(Coordinator *)args;
	Merger *merger=new Merger(COORDINATOR_THERON+1000);
	merger->m_socket();
	char *ipinfo=(char *)malloc(100);
	while(true) {
		stringstream ip_port;
		merger->m_single(ipinfo);
		ip_port<<"tcp://"<<string(ipinfo)<<":"<<WORKER_THERON;
		pthis->aconnect(ip_port.str().c_str());
		Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());
		Logging::getInstance()->getInstance()->log(trace, "one new worker is registering.");
	}
	return 0;
}

bool Coordinator::aconnect(const char * ipport) {
	if(!endpoint_->Connect(ipport)) {
		Logging::getInstance()->getInstance()->log(error, "you need to check your network.");
		return false;
	}
	else {
		Logging::getInstance()->getInstance()->log(trace, ipport);
		Logging::getInstance()->getInstance()->log(trace, "connect a machine.");
		return true;
	}
}
