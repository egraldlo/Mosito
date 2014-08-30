/*
 * m_executor.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_EXECUTOR_H_
#define M_EXECUTOR_H_

class Executor {
public:
	Executor();
	virtual ~Executor();

	static Executor *getInstance();

private:
	static Executor *executor_;
};

#endif /* M_EXECUTOR_H_ */
