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

#include <pthread.h>

pthread_mutex_t lock;
SpineLock spin_lock;
static int ctr=0;

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
