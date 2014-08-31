/*
 * m_task.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TASK_H_
#define M_TASK_H_

#include <iostream>
using namespace std;

typedef void (*Func)(void *args);

class Task {
public:
	Task(Func func,void *args):function_(func),args_(args){};
	virtual ~Task(){};

	void run() {
		function_(args_);
	}

private:
	void (*function_)(void *args);
	void *args_;
};

#endif /* M_TASK_H_ */