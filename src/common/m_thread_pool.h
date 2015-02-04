/*
 * m_thread_pool.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_THREAD_POOL_H_
#define M_THREAD_POOL_H_

#include <pthread.h>

#include "m_circle_queue.h"
#include "m_smart_ptr.h"
#include "m_task.h"

typedef void (*Func)(void *args);

class Task {
public:
	Task(Func func,void *args):function_(func),args_(args) {};
	virtual ~Task() {};

	void run() {
		function_(args_);
	}

private:
	void (*function_)(void *args);
	void *args_;
};

class ThreadPool {
public:
	ThreadPool(int nthreads, int capacity);
	virtual ~ThreadPool();

	static void* single_thread(void * args);
	void add_into_thread_pool(void (*func)(void *), void *args);

	void init_thread_pool();
	void destroy_thread_pool();

private:
	int nthreads_;
	pthread_mutex_t lock_;
	pthread_cond_t condition_;

	CircleQueue<Task> *queue_;
};

class TaskThreadPool {
public:
	TaskThreadPool(int nthreads, int capacity);
	virtual ~TaskThreadPool();

	static void* single_thread(void * args);
	void add_into_thread_pool(TaskInfo *);

	void init_thread_pool();
	void destroy_thread_pool();

private:
	int nthreads_;
	pthread_mutex_t lock_;
	pthread_cond_t condition_;

	CircleQueue<TaskInfo> *queue_;
};

#endif /* M_THREAD_POOL_H_ */
