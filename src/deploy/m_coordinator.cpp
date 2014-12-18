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

	Logging::getInstance()->getInstance()->log(trace, "get local host ip address.");

	const char *ip=inet_ntoa(*(struct in_addr*)hent->h_addr);

	stringstream ip_port;
	ip_port<<"tcp://"<<string(ip)<<":"<<COORDINATOR_THERON;

	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	ostringstream master_ip_port,client_ip_port;

	/*
	 * I have a test on one node, two ports distributed
	 * one is the master,and the other is the client,
	 * definitely you can change the two ports into two ips
	 * to adapt in your PCs.
	 * */
	master_ip_port<<"tcp://"<<"127.0.0.1"<<":5555";
	client_ip_port<<"tcp://"<<"127.0.0.1"<<":5556";

//	Theron::EndPoint endpoint_("coordinator", master_ip_port.str().c_str());
////	endpoint_=new Theron::EndPoint("coordinator", ip_port.str().c_str());
////	pthread_t register_table;
////	/* todo: complete the function. */
////	if(pthread_create(&register_table, 0, register_worker, this)==0) {
////		Logging::getInstance()->getInstance()->log(trace, "register worker thread is running.");
////	}
//
////	Merger *merger=new Merger(COORDINATOR_THERON+1000);
////	merger->m_socket();
////	char *ipinfo=(char *)malloc(100);
////	stringstream ip_port1;
////	merger->m_single(ipinfo);
////	merger->m_close();
////	ip_port1<<"tcp://"<<string(ipinfo)<<":"<<WORKER_THERON;
////	aconnect(ip_port1.str().c_str());
////	Logging::getInstance()->getInstance()->log(trace, ip_port1.str().c_str());
////	Logging::getInstance()->getInstance()->log(trace, "one new worker is registering.");
//
////	aconnect("tcp://127.0.0.1:5556");
////	endpoint_.Connect("tcp://127.0.0.1:5556");
//	if(!endpoint_.Connect(master_ip_port.str().c_str())) {
//		Logging::getInstance()->getInstance()->log(error, "you need to check your network.");
//	}
//	else {
//		Logging::getInstance()->getInstance()->log(trace, "tcp://127.0.0.1:5556");
//		Logging::getInstance()->getInstance()->log(trace, "connect a machine.");
//	}
//
//	Theron::Receiver receiver(endpoint_, "coordinator_receiver");
//	Theron::Framework framework(endpoint_);
////	acn_=new AConnection(*framework, "coordinator_actor");
//
//	AConnection acn(framework, "handler");
//
//	receiver.Wait();


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

		Theron::Receiver *receiver=new Theron::Receiver(clientEndpoint,"clientReceiver");
		Theron::Framework clientFramework(clientEndpoint);
		AConnection *handler=new AConnection(clientFramework,"handler");

		receiver->Wait();
	}

}

//void* Coordinator::register_worker(void *args) {
//	Coordinator *pthis=(Coordinator *)args;
//	Merger *merger=new Merger(COORDINATOR_THERON+1000);
//	merger->m_socket();
//	char *ipinfo=(char *)malloc(100);
//	while(true) {
//		stringstream ip_port;
//		merger->m_single(ipinfo);
//		ip_port<<"tcp://"<<string(ipinfo)<<":"<<WORKER_THERON;
//		pthis->aconnect(ip_port.str().c_str());
//		Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());
//		Logging::getInstance()->getInstance()->log(trace, "one new worker is registering.");
//	}
//
//	return 0;
//}

//bool Coordinator::aconnect(const char * ipport) {
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
