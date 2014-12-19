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
	if(i==1) {
		Coordinator *coordinator=new Coordinator();
		coordinator->init();
	}
	else {
		Worker *worker=new Worker();
		worker->init();
		delete worker;
	}
	return 0;
}
