/*
 * m_coordinator.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_coordinator.h"

Coordinator::Coordinator() {

}

Coordinator::~Coordinator() {

}

void Coordinator::init() {
	/* get the local host ip. */
	char hostname[128];
	struct hostent *hent;

	gethostname(hostname, sizeof(hostname));
	hent=gethostbyname(hostname);

	Logging::getInstance()->getInstance()->log(trace, "[trace]: get local host ip address.");

	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);

	pthread_t register_table;
	/* todo: complete the function. */
	if(pthread_create(&register_table, 0, register_worker, 0)==0) {
		Logging::getInstance()->getInstance()->log(trace, "[trace]: register worker thread is running.");
	}

	stringstream ip_port;
	ip_port<<"tcp://"<<string(ip)<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	Theron::EndPoint *endpoint=new Theron::EndPoint("coordinator", ip_port.str().c_str());
	Theron::Framework *framework=new Theron::Framework(*endpoint);

	acn_=new AConnection(endpoint, *framework, "coordinator_actor");

	getchar();
	getchar();
	getchar();
	getchar();

}

void* Coordinator::register_worker(void *) {
	Merger *merger=new Merger(COORDINATOR_THERON+1000);
	merger->m_socket();
	char *ipinfo=(char *)malloc(100);
	while(true) {
		merger->m_accept();
		merger->m_receive(ipinfo);
		Logging::getInstance()->getInstance()->log(trace, "[trace]: one new worker is registering.");
	}

	return 0;
}
