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

#include "../common/m_task.h"
#include "../common/m_message.h"
#include "../common/m_logging.h"
#include "../physical/m_query_plan.h"
#include "../common/m_thread_pool.h"
using namespace physical;

#include <string>
#include <vector>
#include <sstream>
using namespace std;

class ExecutorSlaveActor;

class ExecutorMaster {
public:
	ExecutorMaster(Theron::EndPoint *end)
	:endpoint_(end) {};
	ExecutorMaster() {};
	virtual ~ExecutorMaster() {};

	static ExecutorMaster* getInstance(Theron::EndPoint *end = 0) {
		if(executormaster_==0) {
			executormaster_=new ExecutorMaster(end);
			return executormaster_;
		}
		else {
			return executormaster_;
		}
	}

	bool sendToMultiple(Message1 &, string);

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
	ExecutorSlave() {
		ttp_=new TaskThreadPool(5,5);
		ttp_->init_thread_pool();
	};
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

	TaskThreadPool *ttp_;
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
		//at here I must new a TaskInfo, it a waste-time error.
		TaskInfo *task=new TaskInfo();
		*task=(TaskInfo::deserialize(message));
		//here we can add the task into the thread pool.
		/* ExecutorSlave::getInstance()->ttp_->add_into_thread_pool(task); */
		pthread_t pths;
		pthread_create(&pths, 0, run, &task);
		cout<<"finished executing the task."<<endl;
	};

	static void *run(void *args) {
		TaskInfo *task=(TaskInfo *)args;
		task->run();
		return 0;
	}
};

#endif /* M_EXECUTOR_H_ */
