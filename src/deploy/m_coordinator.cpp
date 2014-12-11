/*
 * m_coordinator.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_coordinator.h"

Coordinator::Coordinator()
:AConnection("coordinator"){

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

	stringstream ip_port;
	ip_port<<"tcp://"<<string(ip)<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	initialize(ip_port.str().c_str());
}
