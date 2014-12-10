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

Merger::Merger(int nlowers, int port) {
	fd_=0;
	port_=port;
	nlower_=nlowers;
	map_lower_=new int[nlowers];
}

Merger::~Merger() {
	// TODO Auto-generated destructor stub
}

bool Merger::m_socket() {
	struct sockaddr_in my_addr;

	if((fd_=socket(AF_INET, SOCK_STREAM, 0))==-1) {
		Logging::getInstance()->log(error,"[error]: error in socket()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"[trace]: build a socket file descriptor!");
	}

	//todo: add a port manager.
//	int port=111;

	my_addr.sin_port=htons(port_);
	my_addr.sin_family=AF_INET;
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	//bind the fd to the address.
	if(bind(fd_, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1) {
		Logging::getInstance()->log(error,"[error]: error in bind()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"[trace]: bind process well!");
	}

	if(listen(fd_, nlower_)==-1) {
		Logging::getInstance()->log(error,"[error]: error in listen()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"[trace]: listen process well!");
	}

	return true;
}

bool Merger::m_accept() {
	socklen_t size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	int count=0;
	while(count!=nlower_) {
		if((map_lower_[count++]=accept(fd_, (struct sockaddr *)&remote_addr, &size))!=-1) {
			Logging::getInstance()->log(trace,"[trace]: accept a sender node!");
		}
		else {
			Logging::getInstance()->log(error,"[error]: error in accept a connection from sender!");
			return false;
		}
	}
	return true;
}

/* receive a block data from sender, the length is 64KB. */
bool Merger::m_receive(char *data) {
	int ret;
	if((ret=recv(map_lower_[0], data, 100, MSG_WAITALL))==-1) {
		Logging::getInstance()->log(error,"[error]: error in receive data!");
		return false;
	}
	else {
		cout<<"receive data "<<ret<<endl;
		Logging::getInstance()->log(trace,"[trace]: receive data from sender!");
		return true;
	}
}
