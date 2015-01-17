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

//	receiver.Wait(1);

	executor_m_=ExecutorMaster::getInstance(endpoint_);
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
//#ifdef SINGLE_NODE_TEST
//	stringstream file;
//	file<<"table.left."<<Configuration::getInstance()->get_theron_worker_port();
//	ScanSerObj *scan_ser_obj=new ScanSerObj(file.str().c_str());
//#endif
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
	Scan *toser=new Scan(ve,scan_ser_obj);
//	Debug *toser=new Debug(scan);

//	vector<int> uppers;
//	uppers.push_back(1);
//
//#ifndef SINGLE_NODE_TEST
//	vector<int> lowers;
//	lowers.push_back(1);
//#endif
//
//#ifdef SINGLE_NODE_TEST
//	vector<int> lowers;
//	lowers.push_back(5567);
//	lowers.push_back(5568);
//	lowers.push_back(5569);
//	lowers.push_back(5570);
//	lowers.push_back(5571);
//
//#endif
//
//	ShuffleUpperSerObj *suso=new ShuffleUpperSerObj(ve,uppers,lowers,toser,0);
//
//	ShuffleUpper *su=new ShuffleUpper(suso);
//
//	Debug *debug=new Debug(su);
//
//	/* debug can be here for print the shuffleupper data out. */
//
//	debug->prelude();
//	debug->execute(0);
//	debug->postlude();
//
//	getchar();

	vector<int> uppers;
//	uppers.push_back(5567);
	uppers.push_back(5568);

#ifndef SINGLE_NODE_TEST
	vector<int> lowers;
	lowers.push_back(1);
#endif

#ifdef SINGLE_NODE_TEST
	vector<int> lowers;
	lowers.push_back(5569);
	lowers.push_back(5570);
	lowers.push_back(5571);
#endif

	ShuffleUpperSerObj *suso=new ShuffleUpperSerObj(ve,uppers,lowers,toser,0);
	ShuffleUpper *su=new ShuffleUpper(suso);

	vector<int> up;
	up.push_back(1);
	vector<int> low;
//	low.push_back(5567);
	low.push_back(5568);
	ShuffleUpperSerObj *suso1=new ShuffleUpperSerObj(ve,up,low,su,1);
	ShuffleUpper *su1=new ShuffleUpper(suso1);


	Debug *debug=new Debug(su1);
	/* debug can be here for print the shuffleupper data out. */
	debug->prelude();
	debug->execute(0);
	debug->postlude();

	getchar();

}
