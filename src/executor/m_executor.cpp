/*
 * m_executor.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_executor.h"

ExecutorMaster* ExecutorMaster::executormaster_=0;

ExecutorSlave* ExecutorSlave::executorslave_=0;

bool ExecutorMaster::sendToMultiple(QueryPlan * qp, vector<string> ips) {
	/*
	 * executorslave will recieve the task, and deserialize the message
	 * into a task, and executorslave will put the task into the threadpool.
	 *  */
}
