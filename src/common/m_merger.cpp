/*
 * m_merger.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#include "m_merger.h"

Merger::Merger(int nlowers) {
	fd_=0;
	nlower_=nlowers;
	map_lower_=new int[nlowers];
}

Merger::~Merger() {
	// TODO Auto-generated destructor stub
}

bool Merger::m_socket(int nlowers) {
	struct sockaddr_in my_addr;

	if((fd_=socket(AF_INET, SOCK_STREAM, 0))==-1) {
		Logging::getInstance()->log("error in socket()!");
		return false;
	}

	//todo: add a port manager.
	int port=111;
	my_addr.sin_port=htons(port);
	my_addr.sin_family=AF_INET;
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	//bind the fd to the address.
	if(bind(fd_, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1) {
		Logging::getInstance()->log("error in bind()!");
		return false;
	}

	if(listen(fd_, nlowers)==-1) {
		Logging::getInstance()->log("error in listen()!");
		return false;
	}

	return true;
}

bool Merger::m_accept() {
	socklen_t size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	int count=0;
	while(count!=nlower_) {
		if((map_lower_[count++]=accept(fd_, (struct sockaddr *)&remote_addr, &size))!=-1) {
			Logging::getInstance()->log("accept a sender node!");
		}
		else {
			return false;
		}
	}
	return true;
}


