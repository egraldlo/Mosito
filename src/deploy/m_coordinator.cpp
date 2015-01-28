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

//	aconnect(ip_port.str().c_str());

	Theron::Receiver receiver(*endpoint_, "coordinator_receiver");
	Theron::Framework framework(*endpoint_);
//	framework.SetMaxThreads(1);

	acn_= new Thandler(framework, "register");

//	receiver.Wait(1);

	executor_m_=ExecutorMaster::getInstance(endpoint_);
	executor_m_->init_executor();

}

void* Coordinator::register_worker(void *args) {
	Coordinator *pthis=(Coordinator *)args;
	Theron::Framework framework(*pthis->endpoint_);
	Merger *merger=new Merger(COORDINATOR_THERON+1000);
	merger->m_socket();
	char *ipinfo=(char *)malloc(100);
	while(true) {
		stringstream ip_port;
		merger->m_single(ipinfo);
		stringstream new_node;
		new_node<<"worker_"<<string(ipinfo).c_str();
		cout<<"worker endpoint: "<<new_node.str().c_str()<<endl;
		ip_port<<string(ipinfo).c_str();
		pthis->aconnect(ip_port.str().c_str());
		/* send the endpoints_info to the new node and then add it in. */
		for(int i=0; i<pthis->endpoints_info_.size(); i++) {
			MessageIP ip(pthis->endpoints_info_[i].c_str());
			framework.Send(ip, Theron::Address(), Theron::Address(new_node.str().c_str()));
		}
		pthis->endpoints_info_.push_back(ip_port.str());
		Logging::getInstance()->getInstance()->log(error, ip_port.str().c_str());
		Logging::getInstance()->getInstance()->log(error, "one new worker is registering.");
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

void Coordinator::do_join_query() {
	while(1) {

		cout<<"input a enter key and submit a query!"<<endl;
		getchar();
		cout<<"one query has been executed!"<<endl;

		string file="table.left";
		ScanSerObj *scan_ser_obj=new ScanSerObj(file);
		string file1="table.left";
		ScanSerObj *scan_ser_obj1=new ScanSerObj(file1);

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

		vector<DataType *> ve1;
		ve1.push_back(e1);
		ve1.push_back(e2);
		ve1.push_back(e3);
		ve1.push_back(e4);
		ve1.push_back(e5);
		ve1.push_back(e6);
		ve1.push_back(e1);
		ve1.push_back(e2);
		ve1.push_back(e3);
		ve1.push_back(e4);
		ve1.push_back(e5);
		ve1.push_back(e6);

		Scan *toser=new Scan(ve,scan_ser_obj);
		Scan *toser1=new Scan(ve,scan_ser_obj1);

		SortSerObj *sso=new SortSerObj(ve, toser);
		Sort *sort=new Sort(sso);
		SortSerObj *sso1=new SortSerObj(ve, toser1);
		Sort *sort1=new Sort(sso1);

//		vector<int> uppers;
		vector<string> uppers;
		uppers.push_back("10.11.1.198");

	#ifdef SINGLE_NODE_TEST
		vector<string> lowers;
		lowers.push_back("10.11.1.190");
		vector<string> lowers1;
		lowers1.push_back("10.11.1.191");
	#endif


		ShuffleUpperSerObj *suso=new ShuffleUpperSerObj(ve,uppers,lowers,sort,0);
		ShuffleUpper *su=new ShuffleUpper(suso);

		ShuffleUpperSerObj *suso1=new ShuffleUpperSerObj(ve,uppers,lowers1,sort,5);
		ShuffleUpper *su1=new ShuffleUpper(suso1);

		MergeJoinSerObj *mjso=new MergeJoinSerObj(ve,ve,ve1,su,su1);
		QueryPlan *join=new MergeJoin(mjso);

		Debug *debug=new Debug(join);
		/* debug can be here for print the shuffleupper data out. */
		debug->prelude();
		debug->execute(0);
		debug->postlude();

		getchar();
	}

}
