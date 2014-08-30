/*
 * m_executor.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_executor.h"

Executor* Executor::executor_=0;

Executor::Executor() {
	// TODO Auto-generated constructor stub

}

Executor::~Executor() {
	// TODO Auto-generated destructor stub
}

Executor* Executor::getInstance() {
	if(executor_==0) {
		executor_=new Executor();
	}
	return executor_;
}
