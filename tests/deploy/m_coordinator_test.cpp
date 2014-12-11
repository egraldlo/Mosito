/*
 * m_coordinator_test.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Casa
 */

#include "../../src/deploy/m_coordinator.h"

int main_coordinator() {
	Coordinator *coordinator=new Coordinator();
	coordinator->init();
	return 0;
}
