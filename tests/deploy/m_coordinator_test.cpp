/*
 * m_coordinator_test.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "src/deploy/m_coordinator.h"
#include "src/deploy/m_worker.h"

#include <iostream>
#include <sstream>
using namespace std;

#include "../../src/common/m_ahandler.h"

int main_coordinator(int argc, char **argv) {
	int i=atoi(argv[1]);
#ifdef SINGLE_NODE_TEST
	const char *cfg=argv[2];
#endif
	if(i==1) {
#ifdef SINGLE_NODE_TEST
		Configuration::getInstance(cfg);
#endif
		Coordinator *coordinator=new Coordinator();
		coordinator->init();
		coordinator->do_join_query();
	}
	else {
#ifdef SINGLE_NODE_TEST
		Configuration::getInstance(cfg);
#endif
		Worker *worker=new Worker();
		worker->init();
		delete worker;
	}
	return 0;
}
