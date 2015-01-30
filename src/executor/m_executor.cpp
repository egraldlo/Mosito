/*
 * m_executor.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_executor.h"

ExecutorMaster* ExecutorMaster::executormaster_=0;

ExecutorSlave* ExecutorSlave::executorslave_=0;

bool ExecutorMaster::sendToMultiple(Message1& serialized_task, string ips) {
	/*
	 * executorslave will recieve the message and deserialize the messsage to taskinfo,
	 * and executorslave will put the task into the threadpool.
	 *  */
#ifndef SINGLE_NODE_TEST
	for(int slave_id=0; slave_id<ips.size(); slave_id++) {
		/* actor_slave will be add ip: ip+actor_slave. */
		framework_->Send(serialized_task, Theron::Address(), Theron::Address("actor_slave"));
	}
#endif

#ifdef SINGLE_NODE_TEST
	/* actor_slave will be add ip: ip+actor_slave. */
	stringstream actor_name;
	actor_name<<"actor_slave_"<<ips.c_str();
	cout<<"actor_name: "<<actor_name.str().c_str()<<endl;
	framework_->Send(serialized_task, Theron::Address(), Theron::Address(actor_name.str().c_str()));
#endif
	return true;
	/* executorslave must be a theron handler. */
}

void ExecutorMaster::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	Logging::getInstance()->log(trace, "master executor is on!");
}

void ExecutorSlave::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	/* todo: add a id for slaveactor
	 *       at here we can pass a argument to this function and
	 *       construct a slave actor name, so we can simulate distributed
	 *       computing in local node.
	 * */
	Logging::getInstance()->log(trace, "slave executor is on!");
#ifndef SINGLE_NODE_TEST
	/* actor_slave will be add ip: ip+actor_slave. */
	es_actor_=new ExecutorSlaveActor(*framework_, "actor_slave");
#endif

#ifdef SINGLE_NODE_TEST
	stringstream actor_name;
	actor_name<<"actor_slave_"<<Configuration::getInstance()->get_worker_ip().c_str();
	cout<<"actor_name: "<<actor_name.str().c_str()<<endl;
	es_actor_=new ExecutorSlaveActor(*framework_, actor_name.str().c_str());
#endif
	getchar();
}
