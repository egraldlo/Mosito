/*
 * m_coordinator.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#ifndef M_COORDINATOR_H_
#define M_COORDINATOR_H_

#include "../../src/common/m_merger.h"
#include "../../src/common/m_logging.h"
#include "../../src/common/m_ahandler.h"
#include "../../src/executor/m_executor.h"
#include "../../src/common/m_configuration.h"

#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

class Coordinator {
public:
	Coordinator(){};
	virtual ~Coordinator(){};

	void init();

	bool aconnect(const char * ipport);

private:
	static void* register_worker(void *);

private:
	Thandler *acn_;
	ExecutorMaster *executor_m_;

	Theron::EndPoint *endpoint_;

};

#endif /* M_COORDINATOR_H_ */
