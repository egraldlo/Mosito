/*
 * m_thread_pool.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_thread_pool.h"

ThreadPool::ThreadPool(int nthreads,int capacity)
:nthreads_(nthreads),queue_(new CircleQueue<Task>(capacity)) {

}

ThreadPool::~ThreadPool() {
	delete queue_;
}

void *ThreadPool::single_thread(void *args) {
	ThreadPool *tp=reinterpret_cast<ThreadPool *>(args);
	while(true){
		pthread_mutex_lock(&(tp->lock_));
		/*TODO: "IF" will let two ways go, but "WHILE" won't */
		while(tp->queue_->empty()==true){
			pthread_cond_wait(&(tp->condition_),&(tp->lock_));
		}
		/* if not emtpy in task pool, a task can be done here! */
		Task *task=tp->queue_->pop();
		pthread_mutex_unlock(&(tp->lock_));
		task->run();
	}
	/* TODO: how to terminate the single thread. */
	return 0;
}

void ThreadPool::init_thread_pool() {
	pthread_mutex_init(&lock_,0);
	pthread_cond_init(&condition_,0);
	pthread_t *threads_array=new pthread_t[nthreads_];
	for(unsigned i=0;i<nthreads_;i++){
		pthread_create(&threads_array[i],0,single_thread,this);
	}
}

void ThreadPool::destroy_thread_pool() {
	/* TODO: how to terminate the single thread. */
	pthread_cond_destroy(&condition_);
	pthread_mutex_destroy(&lock_);
}

void ThreadPool::add_into_thread_pool(void (*func)(void *), void *args) {
	pthread_mutex_lock(&lock_);
	Task *task=new Task(func,args);
	while(true){
		if(queue_->push(task)==true){
			pthread_cond_signal(&condition_);
			break;
		}
		else{
			/*TODO: ugly code, when full, it will waste CPU in while loop */
			continue;
			usleep(1);
		}
	}
	pthread_mutex_unlock(&lock_);
}
