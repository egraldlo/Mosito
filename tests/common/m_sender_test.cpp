/*
 * m_sender_test.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#include "../../src/common/m_sender.h"
#include "../../src/common/m_merger.h"

#include <stdlib.h>

int sender_local_test(int argc, char **argv) {
	/*
	 * '1' means sender node;
	 * '0' means merger node.
	 *  */
	int sender=atoi((const char *)argv[1]);
	int port=atoi((const char *)argv[2]);
	if(sender) {
		/* this is the sender, it will be executed backforward.*/
		Sender *sender=new Sender(port);

		sender->m_connect("127.0.0.1");

		/* input a string to send. */
		string str;
		cout<<"input a string: "<<endl;
		cin>>str;
		sender->m_send(str.c_str());
	}
	else {
		int nlowers=1;

		Merger *merger=new Merger(nlowers, port);

		merger->m_socket();
		merger->m_accept();

		/* need a memory space to store data. */
		char *data=(char *)malloc(100);
		merger->m_receive(data);
		cout<<data<<endl;
	}
	return 0;
}
