/*
 * m_thread_pool_test.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "../../src/common/m_circle_queue.h"
#include "../../src/common/m_thread_pool.h"
#include "../../src/common/m_task.h"
#include "../../src/common/m_thread_pool.h"

#include <iostream>
using namespace std;

static int print_id=0;

static void print_d_1(void *){
	int print_=print_id++;
	int i=2;
	while(i-->0){
		cout<<"print_id: "<<print_<<endl;
		sleep(2);
	}
}

static void print_dd(void *){
	int print_=print_id++;
	int i=2;
	while(i-->0){
		cout<<"you! "<<print_<<endl;
		sleep(2);
	}
}

class TestThreadPool {
public:
	TestThreadPool(){};
	virtual ~TestThreadPool(){};

	void test_circle(){
		ThreadPool *tp=new ThreadPool(5,10);
		tp->init_thread_pool();
		tp->add_into_thread_pool(&print_d_1,0);
		tp->add_into_thread_pool(&print_d_1,0);
		tp->add_into_thread_pool(&print_dd,0);
		tp->add_into_thread_pool(&print_dd,0);
		tp->add_into_thread_pool(&print_dd,0);
		tp->add_into_thread_pool(&print_dd,0);
		tp->add_into_thread_pool(&print_dd,0);
		tp->add_into_thread_pool(&print_d_1,0);
		tp->add_into_thread_pool(&print_d_1,0);
		tp->add_into_thread_pool(&print_d_1,0);
	};

	void test(){
		cout<<"thread pool test!"<<endl;
	};
};
