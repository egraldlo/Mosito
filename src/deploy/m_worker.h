/*
 * m_worker.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#ifndef M_WORKER_H_
#define M_WORKER_H_

#include "../../src/common/m_sender.h"
#include "../../src/common/m_logging.h"
#include "../../src/common/m_ahandler.h"
#include "../../src/common/m_configuration.h"
#include "../../src/executor/m_executor.h"

#include "../../third_party/theron/Theron/Defines.h"
#include "../../third_party/theron/Theron/Theron.h"

#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class ConnectionEndpoint: public Theron::EndPoint {
public:
	ConnectionEndpoint(const char * const name, const char * const location)
	:Theron::EndPoint(name, location) {
		framework_=new Theron::Framework(*(EndPoint*)this);
	}
//	ConnectionEndpoint(Theron::EndPoint &end):Theron::EndPoint(end) {
//		framework_=new Theron::Framework(end);
//	}
	~ConnectionEndpoint(){};

public:
	Theron::Framework *framework_;
};

class WorkerConnector: public Theron::Actor {
public:
	WorkerConnector(ConnectionEndpoint *endp, const char * const name)
	:Theron::Actor(*(endp->framework_), name), endpoint_(endp){
		Logging::getInstance()->log(trace, "worker connector is on!");
		RegisterHandler(this, &WorkerConnector::handler);
	}

private:
	void handler(const MessageIP &message, const Theron::Address from) {
		cout<<"receive a endpoint of remote node: "<<message.message<<endl;
		endpoint_->Connect(message.message);
		cout<<"finished connecting with a remote node."<<endl;
	};

private:
	ConnectionEndpoint *endpoint_;
};

class Worker {
public:
	Worker(){};
	virtual ~Worker(){};

	void init();

private:

private:
	Thandler *acn_;
	ExecutorMaster *executor_m_;
	ExecutorSlave *executor_s_;

	Theron::EndPoint *endpoint_;
	WorkerConnector *worker_connector_;

};

#endif /* M_WORKER_H_ */
