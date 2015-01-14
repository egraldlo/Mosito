/*
 * m_executor.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_executor.h"

ExecutorMaster* ExecutorMaster::executormaster_=0;

ExecutorSlave* ExecutorSlave::executorslave_=0;

bool ExecutorMaster::sendToMultiple(Message1 msg, vector<int> ips) {
	/*
	 * executorslave will recieve the message and deserialize the messsage to taskinfo,
	 * and executorslave will put the task into the threadpool.
	 *  */
	for(int slave_id=0; slave_id<ips.size(); slave_id++) {
		framework_->Send(msg, Theron::Address(), Theron::Address("slaveactor"));
	}

	/* executorslave must be a theron handler. */
}

void ExecutorMaster::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	Logging::getInstance()->log(trace, "master executor is on!");
}

void ExecutorSlave::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	Logging::getInstance()->log(trace, "slave executor is on!");
	/* todo: add a id for slaveactor */
	es_actor_=new ExecutorSlaveActor(*framework_, "slaveactor");
	getchar();
}
