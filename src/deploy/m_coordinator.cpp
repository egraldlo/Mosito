/*
 * m_coordinator.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "m_coordinator.h"

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

	Logging::getInstance()->getInstance()->log(trace, "my ip and port is: ");
	Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());

	endpoint_=new Theron::EndPoint("coordinator", ip_port.str().c_str());
	pthread_t register_table;
	/* todo: complete the function. */
	if(pthread_create(&register_table, 0, register_worker, this)==0) {
		Logging::getInstance()->getInstance()->log(trace, "register worker thread is running.");
	}

	aconnect(ip_port.str().c_str());

	Theron::Receiver receiver(*endpoint_, "coordinator_receiver");
	Theron::Framework framework(*endpoint_);
	framework.SetMaxThreads(1);

	acn_= new Thandler(framework, "register");

	receiver.Wait(1);

	executor_m_=new ExecutorMaster(endpoint_);
	executor_m_->init_executor();

}

void* Coordinator::register_worker(void *args) {
	Coordinator *pthis=(Coordinator *)args;
	Merger *merger=new Merger(COORDINATOR_THERON+1000);
	merger->m_socket();
	char *ipinfo=(char *)malloc(100);
	while(true) {
		stringstream ip_port;
		merger->m_single(ipinfo);
		ip_port<<"tcp://"<<string(ipinfo)<<":"<<WORKER_THERON;
		pthis->aconnect(ip_port.str().c_str());
		Logging::getInstance()->getInstance()->log(trace, ip_port.str().c_str());
		Logging::getInstance()->getInstance()->log(trace, "one new worker is registering.");
	}
	return 0;
}

bool Coordinator::aconnect(const char * ipport) {
	if(!endpoint_->Connect(ipport)) {
		Logging::getInstance()->getInstance()->log(error, "you need to check your network.");
		return false;
	}
	else {
		Logging::getInstance()->getInstance()->log(trace, ipport);
		Logging::getInstance()->getInstance()->log(trace, "connect a machine.");
		return true;
	}
}

void Coordinator::do_query() {
	/*
	 * todo: two level query which has one node and two nodes.
	 * 		 Scan operator + ShuffleLower + ShuffleUpper + Debug.
	 * 		 so there is shuffle which can cover the query.
	 *
	 * 		 one node use coordinator and other two nodes use worker,
	 * 		 different port can be used for simulation.
	 *  */

	cout<<"input a enter key and submit a query!"<<endl;
	getchar();
	cout<<"one query has been executed!"<<endl;

	string file="table.left";
	ScanSerObj *scan_ser_obj=new ScanSerObj(file);
	DataType *e1=new UnLongType(t_long);
	DataType *e2=new IntegerType(t_int);
	DataType *e3=new IntegerType(t_int);
	DataType *e4=new IntegerType(t_int);
	DataType *e5=new IntegerType(t_int);
	DataType *e6=new IntegerType(t_int);
	vector<DataType *> ve;
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	ve.push_back(e6);
	Scan *scan=new Scan(ve,scan_ser_obj);
	Debug *debug=new Debug(scan);

	vector<int> uppers;
	uppers.push_back(1);
	vector<int> lowers;
	lowers.push_back(1);

	ShuffleUpperSerObj *suso=new ShuffleUpperSerObj(ve,uppers,lowers,scan);
	ShuffleUpper *su=new ShuffleUpper(suso);

	su->prelude();
	su->execute(0);
	su->postlude();

	getchar();

}
