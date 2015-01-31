/*
 * m_merger.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#include "m_merger.h"

Merger::Merger(int port) {
	fd_=0;
	port_=port;
	/* nlower is not limited. */
	nlower_=2;
	map_lower_=new int[nlower_];
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
		Logging::getInstance()->log(error,"error in socket()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"build a socket file descriptor!");
	}

	//todo: add a port manager.
//	int port=111;

	my_addr.sin_port=htons(port_);
	my_addr.sin_family=AF_INET;
	my_addr.sin_addr.s_addr=INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	//bind the fd to the address.
	if(bind(fd_, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1) {
		Logging::getInstance()->log(error,"error in bind()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"bind process well!");
	}

	if(listen(fd_, nlower_)==-1) {
		Logging::getInstance()->log(error,"error in listen()!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"listen process well!");
	}

	data_=new char[BLOCK_SIZE];
	debug_count_=0;
	meet_zero_=0;

	return true;
}

bool Merger::m_accept() {
	socklen_t size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	int count=0;
	while(count!=nlower_) {
		if((map_lower_[count++]=accept(fd_, (struct sockaddr *)&remote_addr, &size))!=-1) {
			Logging::getInstance()->log(trace,"accept a sender node!");
		}
		else {
			Logging::getInstance()->log(error,"error in accept a connection from sender!");
			return false;
		}
	}
	return true;
}

/* receive a block data from sender, the length is 64KB. */
bool Merger::m_receive(char *data) {
	int ret;
	if((ret=recv(map_lower_[0], data, BLOCK_SIZE, MSG_WAITALL))==-1) {
		Logging::getInstance()->log(error,"error in receive data!");
		return false;
	}
	else {
		if(ret!=0) {
			cout<<"receive data "<<ret<<" data: "<<data<<endl;
			Logging::getInstance()->log(trace,"receive data from sender!");
			return true;
		}
		else {
			/* todo: a better way to handle with the border. */
			Logging::getInstance()->log(trace,"sender finish transferring data!");
			return false;
		}
	}
}

bool Merger::m_single(char * data) {
	socklen_t size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	int retfd;
	if((retfd=accept(fd_, (struct sockaddr *)&remote_addr, &size))!=-1) {
		Logging::getInstance()->log(trace,"accept a sender node!");
	}
	else {
		Logging::getInstance()->log(error,"error in accept a connection from sender!");
		return false;
	}

	int ret;
	if((ret=recv(retfd, data, 100, MSG_WAITALL))==-1) {
		Logging::getInstance()->log(error,"error in receive data!");
		return false;
	}
	else {
		Logging::getInstance()->log(trace,"receive data from sender!");
	}
	return true;

}

bool Merger::m_close() {
	free(data_);
	close(fd_);
	return true;
}

bool Merger::m_receive_select(PCBuffer *pcbuffer) {
	/*
	 * in a simple version, we try to use select because that number of connections is small,
	 * to put the blocks the merger receive into the producer-consumer buffer.
	 *  */
	fd_set fds;

	Block *block=new Block(BLOCK_SIZE, pcbuffer->getSchema().totalsize_);

	while(1) {
		/* set the fds empty. */
		FD_ZERO(&fds);
		int maxfd=0;
		for(int i=0; i<nlower_; i++) {
			/* set the fds which will be listened in fds. */
			FD_SET(map_lower_[i], &fds);
			if(map_lower_[i]>maxfd) {
				maxfd=map_lower_[i];
			}
		}

		Logging::getInstance()->getInstance()->log(trace, "hello? this is the select syscall.");

		/* select will return the number of active fds. */
		switch(select(maxfd+1, &fds, 0, 0, (timeval *)0)) {
		case -1: exit(-1); break;
		case 0: {cout<<"warmming!!"<<endl;break;}
		default:
			for(int i=0; i<nlower_; i++) {
				if(FD_ISSET(map_lower_[i], &fds)) {
					int ret;
					/* receive the data from the sender. */
					if((ret=recv(map_lower_[i], data_, BLOCK_SIZE, MSG_WAITALL))==-1) {
						Logging::getInstance()->log(error,"error in receive data!");
					}
					/*
					 * construct a block from the data and put it into the pcbuffer.
					 * todo: here the size of data_ is not BLOCK_SIZE.
					 * */
					Logging::getInstance()->log(trace, "store the data into the block.");
					block->reset();
					block->storeBlock(data_, BLOCK_SIZE);
					Logging::getInstance()->log(trace, "put the block into the pc_buffer.");
					pcbuffer->put(block, i);
					stringstream debug_co;
					if(block->get_size()==0) {
						if(++meet_zero_==nlower_) {
							m_close();
							return false;
						}
					}
					debug_co<<"the deubg count number is: "<<debug_count_++;
					Logging::getInstance()->log(trace, debug_co.str().c_str());
				}
			}
		}
	}

	return true;
}
