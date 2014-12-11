/*
 * m_coordinator.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#ifndef M_COORDINATOR_H_
#define M_COORDINATOR_H_

#include "../../src/common/m_logging.h"
#include "../../src/common/m_ahandler.h"
#include "../../src/common/m_configuration.h"

#include <sstream>
using namespace std;

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class Coordinator: public AConnection {
public:
	Coordinator();
	virtual ~Coordinator();

	void init();

private:

};

#endif /* M_COORDINATOR_H_ */
