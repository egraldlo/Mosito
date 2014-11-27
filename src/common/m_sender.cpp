/*
 * m_sender.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#include "m_sender.h"

Sender::Sender() {
	fd_=0;
}

Sender::~Sender() {

}

bool Sender::m_socket(string ip, int port) {
	if((fd_=socket(AF_INET, SOCK_STREAM, 0))==-1) {
		Logging::getInstance()->log("error in socket()!");
	}

	struct sockaddr_in serv;
	serv.sin_addr.s_addr=inet_addr(ip.c_str());
	serv.sin_family=AF_INET;
	serv.sin_port=htons(port);
	bzero(&(serv.sin_zero),8);

	if((connect(fd_, (struct sockaddr *)&serv, sizeof(struct sockaddr)))==-1) {
		Logging::getInstance()->log("error in connect()!");
	}

	return true;
}
