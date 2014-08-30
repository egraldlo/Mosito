/*
 * m_synchronize.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_synchronize.h"

Lock::Lock() {
	pthread_mutex_init(&lock,0);
}

Lock::~Lock() {
	pthread_mutex_destroy(&lock);
}

void Lock::acquire() {
	pthread_mutex_lock(&lock);
}

void Lock::release() {
	pthread_mutex_unlock(&lock);
}

Semaphore::Semaphore(int n) {
	sem_init(&semaphore,0,n);
}

Semaphore::~Semaphore() {
	sem_destroy(&semaphore);
}

void Semaphore::post() {
	sem_post(&semaphore);
}

void Semaphore::wait() {
	sem_wait(&semaphore);
}

Barrier::Barrier(int n) {
	nthreads=n;
	if(pthread_mutex_init(&lock,0)==0)
		cout<<"init the mutex lock error!"<<endl;
	if(pthread_cond_init(&condition,0)==0)
		cout<<"init the condition error!"<<endl;
	ncounts=0;
}

Barrier::~Barrier() {
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&condition);
}

void Barrier::arrive() {
	pthread_mutex_lock(&lock);
	ncounts++;
	if(ncounts<nthreads) {
		pthread_cond_wait(&condition,&lock);
	}
	else{
		pthread_cond_broadcast(&condition);
	}
	pthread_mutex_unlock(&lock);
}
