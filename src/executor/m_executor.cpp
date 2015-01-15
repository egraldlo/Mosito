/*
 * m_executor.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_executor.h"

ExecutorMaster* ExecutorMaster::executormaster_=0;

ExecutorSlave* ExecutorSlave::executorslave_=0;

bool ExecutorMaster::sendToMultiple(QueryPlan *qp, vector<int> ips) {
	/*
	 * executorslave will recieve the message and deserialize the messsage to taskinfo,
	 * and executorslave will put the task into the threadpool.
	 *  */
	TaskInfo tasks(qp);
	Message1 serialized_task=TaskInfo::serialize(tasks);
	Logging::getInstance()->log(trace, "ready for send the task to multiple nodes.");
	for(int slave_id=0; slave_id<ips.size(); slave_id++) {
		framework_->Send(serialized_task, Theron::Address(), Theron::Address("actor_slave"));
	}

	return true;
	/* executorslave must be a theron handler. */
}

void ExecutorMaster::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	Logging::getInstance()->log(trace, "master executor is on!");
}

void ExecutorSlave::init_executor() {
	framework_=new Theron::Framework(*endpoint_);
	Logging::getInstance()->log(trace, "slave executor is on!");
	/* todo: add a id for slaveactor
	 *       at here we can pass a argument to this function and
	 *       construct a slave actor name, so we can simulate distributed
	 *       computing in local node.
	 * */
	es_actor_=new ExecutorSlaveActor(*framework_, "actor_slave");
	getchar();
}
