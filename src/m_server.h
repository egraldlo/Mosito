/*
 * m_server.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SERVER_H_
#define M_SERVER_H_

#include <iostream>
using namespace std;

class Server {
public:
	Server();
	virtual ~Server();

	void initialize(){cout<<"init the server!"<<endl;};
};

int main(){
	Server *server=new Server();
	server->initialize();

	return 0;
}

#endif /* M_SERVER_H_ */
