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

#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class Worker {
public:
	Worker();
	virtual ~Worker();

	void init();

private:
	AConnection *acn_;
};

#endif /* M_WORKER_H_ */
