/*
 * m_synchronize.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SYNCHRONIZE_H_
#define M_SYNCHRONIZE_H_

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <iostream>
using namespace std;

class Lock{
public:
	Lock();
	virtual ~Lock();

	void acquire();
	void release();

private:
	pthread_mutex_t lock;
};

class Semaphore{
public:
	Semaphore(int n);
	virtual ~Semaphore();

	void post();
	void wait();

private:
	sem_t semaphore;
};

class Barrier{
public:
	Barrier(int n);
	virtual ~Barrier();

	void arrive();

private:
	int nthreads;
	pthread_mutex_t lock;
	pthread_cond_t condition;
	/* volatile avoid compiler optimization. */
	volatile int ncounts;
};

#endif /* M_SYNCHRONIZE_H_ */
