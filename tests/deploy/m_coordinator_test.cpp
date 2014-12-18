/*
 * m_coordinator_test.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "src/deploy/m_coordinator.h"
#include "src/deploy/m_worker.h"

#include "../../src/common/m_ahandler.h"
#include "../../src/common/m_message.h"

#include "../../third_party/theron/Theron/Theron.h"
#include "../../third_party/theron/Theron/Defines.h"

int main_coordinator(int argc, char **argv) {
//	int i=atoi(argv[1]);
//	if(i==1) {
//		Coordinator *coordinator=new Coordinator();
//		coordinator->init();
//	}
//	else {
//		Worker *worker=new Worker();
//		worker->init();
//	}

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
		AConnection han(clientFramework,"handler");

		receiver.Wait();
	}

	return 0;
}
