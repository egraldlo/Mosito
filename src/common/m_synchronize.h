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

class Lock {
public:
	Lock();
	virtual ~Lock();

	void acquire();
	void release();

private:
	pthread_mutex_t lock;
};

class SpineLock {
	public:
	SpineLock() : _l(0) { }

	/** Call blocks and retunrs only when it has the lock. */
	inline void acquire()
	{
		while(tas(&_l)) {
#if defined(__i386__) || defined(__x86_64__)
			__asm__ __volatile__ ("pause\n");
#endif
		}
	}
	inline bool try_lock()
	{
		return !tas(&_l);
	}
	inline int fakelock()
	{
		return _l==0?0:1;
	}
	inline int getvalue()
	{
		//return _l==0?0:1;
		return 0;
	}

	/** Unlocks the lock object. */
	inline void release()
	{
		_l = 0;
	}

private:
	inline int tas(volatile char* lock)
	{
		register char res = 1;
#if defined(__i386__) || defined(__x86_64__)
		__asm__ __volatile__ (
				"lock xchgb %0, %1\n"
				: "+q"(res), "+m"(*lock)
				:
				: "memory", "cc");
#elif defined(__sparc__)
		__asm__ __volatile__ (
				"ldstub [%2], %0"
				: "=r"(res), "+m"(*acquire)
				: "r"(acquire)
				: "memory");
#else
#error TAS not defined for this architecture.
#endif
		return res;
	}

	volatile char _l ;

};

class RWLock {
public:
	RWLock();
	virtual ~RWLock();

	void acquire_r();
	void acquire_w();

	void release();

private:
	pthread_rwlock_t rw_lock;
};

class Semaphore {
public:
	Semaphore(int n);
	virtual ~Semaphore();

	void post();
	void wait();

private:
	sem_t semaphore;
};

class Barrier {
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
