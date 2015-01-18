/*
 * m_coordinator.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#ifndef M_COORDINATOR_H_
#define M_COORDINATOR_H_

#include "../../src/common/m_task.h"
#include "../../src/common/m_merger.h"
#include "../../src/common/m_logging.h"
#include "../../src/common/m_ahandler.h"
#include "../../src/common/m_configuration.h"
#include "../../src/executor/m_executor.h"
#include "../../src/physical/m_scan.h"
#include "../../src/physical/m_shuffle_upper.h"

#include "../../third_party/theron/Theron/Defines.h"
#include "../../third_party/theron/Theron/Theron.h"

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

	/* this function is for test. */
	void do_query();

private:
	static void* register_worker(void *);

private:
	vector<string> endpoints_info_;

	Thandler *acn_;
	ExecutorMaster *executor_m_;

	Theron::EndPoint *endpoint_;

};

#endif /* M_COORDINATOR_H_ */
