/*
 * m_executor.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_EXECUTOR_H_
#define M_EXECUTOR_H_

//#include "../../third_party/theron/Theron/Defines.h"
//#include "../../third_party/theron/Theron/Theron.h"

#include "../../src/common/m_task.h"
#include "../common/m_message.h"
#include "../common/m_logging.h"
#include "../physical/m_query_plan.h"
using namespace physical;

#include <string>
#include <vector>
using namespace std;

class ExecutorSlaveActor;

class ExecutorMaster {
public:
	ExecutorMaster(Theron::EndPoint *end)
	:endpoint_(end) {};
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

	bool sendToMultiple(Message1, vector<int>);

	void init_executor();

private:
	static ExecutorMaster* executormaster_;
	Theron::Framework *framework_;
	Theron::EndPoint *endpoint_;

};

class ExecutorSlave {
public:
	ExecutorSlave(Theron::EndPoint *end)
	:endpoint_(end) {};
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

	void init_executor();

private:
	static ExecutorSlave *executorslave_;
	ExecutorSlaveActor *es_actor_;
	Theron::Framework *framework_;
	Theron::EndPoint *endpoint_;

};

/* this actor can be put into the common src. */
class ExecutorSlaveActor: public Theron::Actor {
public:
	ExecutorSlaveActor(Theron::Framework &framework, const char * const name)
	:Theron::Actor(framework, name){
		RegisterHandler(this, &ExecutorSlaveActor::handler);
	}

private:
	void handler(const Message1 &message, const Theron::Address from) {
		cout<<"hello, the task is: "<<message.message<<endl;
		TaskInfo task=TaskInfo::deserialize(message);
		task.run();
		cout<<"finished executing the task."<<endl;
	};

};

#endif /* M_EXECUTOR_H_ */
