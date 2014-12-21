/*
 * m_executor.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_EXECUTOR_H_
#define M_EXECUTOR_H_

#include "../physical/m_query_plan.h"
using namespace physical;

#include <string>
#include <vector>
using namespace std;

class ExecutorMaster {
public:
	ExecutorMaster() {};
	virtual ~ExecutorMaster() {};

	static ExecutorMaster* getInstance() {
		if(executormaster_==0) {
			executormaster_=new ExecutorMaster();
			return executormaster_;
		}
		else {
			return executormaster_;
		}
	}

	bool sendToMultiple(QueryPlan *qp, vector<string> ips);

private:
	static ExecutorMaster* executormaster_;
};

class ExecutorSlave {
public:
	ExecutorSlave() {};
	virtual ~ExecutorSlave() {};

	static ExecutorSlave *getInstance() {
		if(executorslave_==0) {
			executorslave_=new ExecutorSlave();
			return executorslave_;
		}
		else {
			return executorslave_;
		}
	}

private:
	static ExecutorSlave *executorslave_;
};

#endif /* M_EXECUTOR_H_ */
