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

Sender::Sender(int port) {
	port_=port;
}

Sender::~Sender() {

}

/* ip and port can be managed by a more intelligent. */
bool Sender::m_connect(string ip) {
	if((fd_=socket(AF_INET, SOCK_STREAM, 0))==-1) {
		Logging::getInstance()->log(error,"error in socket()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"build a socket file descripter!");
	}

	struct hostent *host;
	/* gethostbyname is in netdb.h and sys/socket.h*/
	if((host=gethostbyname(ip.c_str()))==0) {
		Logging::getInstance()->log(error,"error in get host by name!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"get host by name!");
	}

	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	serv.sin_port=htons(port_);
	/* in_addr in netinet/in.h */
	serv.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv.sin_zero),8);

	if((connect(fd_, (struct sockaddr *)&serv, sizeof(struct sockaddr)))==-1) {
		Logging::getInstance()->log(error,"error in connect()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"connect the merger successfully!");
	}

	return true;
}

/* a block must be sent, the length will be about 64KB.*/
bool Sender::m_send(const char *data, unsigned size) {
	int ret;
	if((ret=send(fd_, data, size, 0))==-1) {
		Logging::getInstance()->log(error,"error in sending data!");
		return false;
	}
	else {
		if(ret==size){
			Logging::getInstance()->log(trace,"sending data successfully!");
			return true;
		}
		else {
			Logging::getInstance()->log(error,"error in sending data!");
			return false;
		}
	}
}

bool Sender::m_close() {
	close(fd_);
	return true;
}
