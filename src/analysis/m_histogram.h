/*
 * m_histogram.h
 *
 *  Created on: Feb 3, 2015
 *      Author: Casa
 */

#ifndef M_HISTOGRAM_H_
#define M_HISTOGRAM_H_

#include "../common/m_configuration.h"
#include "../common/m_synchronize.h"
#include "../common/m_buffer.h"
#include "../common/m_timer.h"

#include <stdio.h>
#include <pthread.h>

#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Hist {
public:
	Hist() {
//		sl_=new SpineLock();
		pthread_mutex_init(&lock_, 0);
		counter_=0;
	}
	~Hist() {
		pthread_mutex_destroy(&lock_);
	}

	void increase() {
//		sl_->acquire();
		pthread_mutex_lock(&lock_);
		counter_++;
//		sl_->release();
		pthread_mutex_unlock(&lock_);
	}

	pthread_mutex_t lock_;
//	SpineLock *sl_;
	unsigned counter_;
};

class Histogram {
public:
	Histogram();
	virtual ~Histogram();

	void eval(const char *);
	void getbound(unsigned long&, unsigned long&);

//	void pthreads_eval(const char *);
//	static void *pthread_eval(void *args);

//	struct Args {
//		Histogram *ts;
//		int i;
//	};

private:
	int histogram_[HIST_SIZE];

public:
//	hist **hist_;
	vector<Block *> blocks_;
//	pthread_t pths_[CPU_FRE];
};

class Histogram1 {
public:
	Histogram1(){
		hist_=new Hist*[HIST_SIZE];
		for(unsigned i=0; i<HIST_SIZE; i++) {
			hist_[i]=new Hist();
		}
	}
	virtual ~Histogram1() {}

	void pthreads_eval(const char *);
	static void *pthread_e(void *args);
	static void *bound(void *args);
	void getbound(unsigned long&, unsigned long&);

	struct Args {
		Histogram1 *ts;
		int i;
	};

	struct Args1 {
		Histogram1 *ts;
		int i;
	};

public:
	Hist **hist_;
	vector<Block *> blocks_;
	unsigned long start_[CPU_CORE];
	unsigned long end_[CPU_CORE];

private:
	pthread_t pths_[CPU_CORE];
};

#endif /* M_HISTOGRAM_H_ */
