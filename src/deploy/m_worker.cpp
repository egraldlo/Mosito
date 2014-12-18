/*
 * m_worker.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_worker.h"

Worker::Worker() {

}

Worker::~Worker() {

}

void Worker::init() {
	/* get the local host ip. */
//	char hostname[128];
//	struct hostent *hent;
//
//	gethostname(hostname, sizeof(hostname));
//	hent=gethostbyname(hostname);
//
//	Logging::getInstance()->getInstance()->log(trace, "get local host ip address.");
//
//	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);
//
//	stringstream ip_port_worker,ip_port_master;
//	ip_port_worker<<"tcp://"<<string(ip)<<":"<<WORKER_THERON;
//	/* todo: solve the master ip problem. */
//	ip_port_master<<"tcp://"<<string("127.0.0.1")<<":"<<COORDINATOR_THERON;
//
//	Logging::getInstance()->getInstance()->log(trace, ip_port_worker.str().c_str());

	ostringstream master_ip_port,client_ip_port;

	/*
	 * I have a test on one node, two ports distributed
	 * one is the master,and the other is the client,
	 * definitely you can change the two ports into two ips
	 * to adapt in your PCs.
	 * */
	master_ip_port<<"tcp://"<<"127.0.0.1"<<":6555";
	client_ip_port<<"tcp://"<<"127.0.0.1"<<":6556";

//	/* todo: a id manager to give a id to worker. */
//	Theron::EndPoint endpoint_("worker", client_ip_port.str().c_str());
////	endpoint_=new Theron::EndPoint("worker", ip_port_worker.str().c_str());
//
////	aconnect("tcp://127.0.0.1:5555");
//	if(!endpoint_.Connect(client_ip_port.str().c_str())) {
//		Logging::getInstance()->getInstance()->log(error, "you need to check your network.");
//	}
//	else {
//		Logging::getInstance()->getInstance()->log(trace, "tcp://127.0.0.1:5555");
//		Logging::getInstance()->getInstance()->log(trace, "connect a machine.");
//	}
////	aconnect(ip_port_master.str().c_str());
//
////	Sender *sender=new Sender(COORDINATOR_THERON+1000);
////	sender->m_connect("127.0.0.1");//master
////	sender->m_send(ip);
////	sender->m_close();
//
//	printf("hello sending ---------------->\n");
//
//
//	Theron::Framework framework(endpoint_);
//	sleep(1);
////	acn_=new AConnection(*framework, "worker_actor");
//	framework.Send(Message1("hello"), Theron::Address(), Theron::Address("handler"));

	int m_c=0;
	scanf("%d",&m_c);
	if(m_c==1){
		/*
		 * port 5555 master
		 * */
		Theron::EndPoint masterEndpoint("master",master_ip_port.str().c_str());

		if(!masterEndpoint.Connect(client_ip_port.str().c_str())){
			cout<<"check your network!"<<endl;
		}
		Theron::Framework masterFramework(masterEndpoint);
        sleep(1);
        /*I have a test that shows this "getchar()" is needed,
         * you can have a try. :)
         */
        masterFramework.Send(MessageT("hello"),Theron::Address(),Theron::Address("handler"));
	}else{
		/*
		 * port 5556 client
		 * */
		Theron::EndPoint clientEndpoint("client",client_ip_port.str().c_str());

		if(!clientEndpoint.Connect(master_ip_port.str().c_str())){
			cout<<"check your network!"<<endl;
		}

		Theron::Receiver receiver(clientEndpoint,"clientReceiver");
		Theron::Framework clientFramework(clientEndpoint);
		AConnection handler(clientFramework,"handler");

		receiver.Wait();
	}

}

//bool Worker::aconnect(const char * ipport) {
//	if(!endpoint_.Connect(ipport)) {
//		Logging::getInstance()->getInstance()->log(error, "you need to check your network.");
//		return false;
//	}
//	else {
//		Logging::getInstance()->getInstance()->log(trace, ipport);
//		Logging::getInstance()->getInstance()->log(trace, "connect a machine.");
//		return true;
//	}
//}
