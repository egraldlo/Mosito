/*
 * m_synchronize_test.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "../../src/common/m_synchronize.h"

#define NUM 500
#define PER 10000
//#define MUTEX
#define SPIN_LOCK
#define RW_LOCK

#include <pthread.h>

pthread_mutex_t lock;
SpineLock spin_lock;
RWLock rw_lock;
static int ctr=0;
static int ctr_rw=0;

struct thread_info{
	int counter;
};

void *func(void *attr) {
	thread_info *att=(thread_info*)attr;
	while(att->counter++<PER) {
#ifdef MUTEX
	pthread_mutex_lock(&lock);
#endif
#ifdef SPIN_LOCK
	spin_lock.acquire();
#endif
	ctr++;
#ifdef MUTEX
	pthread_mutex_unlock(&lock);
#endif
#ifdef SPIN_LOCK
	spin_lock.release();
#endif
	}
	return 0;
}

int main_lock() {
	pthread_t *threads;
	threads=new pthread_t[NUM];

	thread_info *tinfo;
	tinfo=new thread_info[NUM];

	int err=0;

	pthread_mutex_init(&lock,0);

	for(unsigned i=0; i<NUM; i++) {
		tinfo[i].counter=0;
		if((err=pthread_create(&threads[i],0,func,&tinfo[i]))==0) {
			cout<<"error: "<<err<<endl;
		}
	}

	for(unsigned j=0; j<NUM; j++) {
		cout<<"threads[j]: "<<threads[j]<<endl;
	}

	for(unsigned k=0; k<NUM; k++) {
		pthread_join(threads[k],0);
	}

	cout<<"all is over! ctr: "<<ctr<<endl;

	return 0;
}

void *read_(void * attr) {
	thread_info *att=(thread_info*)attr;
	while(true) {
		usleep(100);
	#ifdef MUTEX
		pthread_mutex_lock(&lock);
	#endif
	#ifdef RW_LOCK
		rw_lock.acquire_r();
	#endif
		if(ctr_rw<50000)
			cout<<"["<<att->counter<<"]"<<" : "<<ctr_rw<<endl;
		else{
	#ifdef MUTEX
			pthread_mutex_unlock(&lock);
	#endif
	#ifdef RW_LOCK
			rw_lock.release();
	#endif
			break;
		}
	#ifdef MUTEX
			pthread_mutex_unlock(&lock);
	#endif
	#ifdef RW_LOCK
			rw_lock.release();
	#endif
	}
	return 0;
}

void *write_(void *) {
	while(ctr_rw<50000) {
#ifdef MUTEX
	pthread_mutex_lock(&lock);
#endif
#ifdef RW_LOCK
	rw_lock.acquire_w();
#endif
	ctr_rw++;
#ifdef MUTEX
	pthread_mutex_unlock(&lock);
#endif
#ifdef RW_LOCK
	rw_lock.release();
#endif
	}
	return 0;
}

int main_rwlock() {
	/*
	 * test that five read threads and one write thread
	 * in the read-write lock mode and lock mode.
	 *  */
	int nthreads=10;

	pthread_t *threads;
	threads=new pthread_t[nthreads];

	thread_info *tinfo;
	tinfo=new thread_info[NUM];

	pthread_t thread;

	pthread_mutex_init(&lock,0);

	for(unsigned i=0; i<nthreads; i++) {
		tinfo[i].counter=i;
		pthread_create(&threads[i], 0, read_, &tinfo[i]);
	}

	pthread_create(&thread, 0, write_, 0);

	for(unsigned j=0; j<nthreads; j++) {
		pthread_join(threads[j],0);
	}

	sleep(1);
	pthread_join(thread,0);
	return 0;
}
