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
	/* constructor can be private, but here we know that we only use getInstance. */
	Executor();
	virtual ~Executor();

	/* the only interface we use to create or get the instance. */
	static Executor *getInstance();

private:
	static Executor *executor_;
};

#endif /* M_EXECUTOR_H_ */
