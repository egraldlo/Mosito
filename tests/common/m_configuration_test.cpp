/*
 * m_configuration_test.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "../../src/common/m_configuration.h"

#include <iostream>
using namespace std;

int main_configuration() {
	cout<<"the coordinator ip is: "<<Configuration::getInstance()->get_coordinator_ip().c_str()<<endl;
	cout<<"the theron port is: "<<Configuration::getInstance()->get_theron_worker_port()<<endl;
	return 0;
}
