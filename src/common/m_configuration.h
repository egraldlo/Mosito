/*
 * m_configuration.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_CONFIGURATION_H_
#define M_CONFIGURATION_H_

/*
 * this configuration class will read the config file by using
 * the libconfig lib, like ip and port.
 *  */

#include <libconfig.h++>

#include <iostream>
using namespace std;

/* provide INT_MIN, INT_MAX etc. */
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

/* block size of pipeline. */
#define BLOCK_SIZE 64*1024
/* initial flex block size. */
#define INIT_FLEX_BLOCK_SIZE 64
/* this is the heap size to sort the stream. */
#define HEAP_SIZE 100
/* buffer the data which is from the heap. */
#define HEAP_SORT_BUFFER_INIT_SIZE 1024
/* every loser tree leaf node buffer. */
#define LOOSE_TREE_BUFFER_NO 10
/* heap sort buffer increasing factor. */
#define HEAP_SORT_BUFFER_INCREASING_FACTOR 2
/* coordinator theron port */
#define COORDINATOR_THERON 5545
/* worker theron port */
#define WORKER_THERON 5546
/* port base for test */
#define PORT_BASE 6689

/* every column in the PC buffer has COLUMN_CON blocks */
#define COLUMN_CON 20

/* this is my experiment */
#define EXPERIMENT
#define EXPERIMENT_TEST
#define TIMING
#define SINGLE_NODE_TEST
#define MULTI_PARTITION

/* 256*1024 histogram is 1 MB */
#define HIST_SIZE 262144

/* configure log type. */
#define LOGGER error

/* the cpu core of the machine. */
#define CPU_CORE 4
#define MUL 4

static int counter=0;

static void print_tuple(void *sta) {
	printf("%7d ++",counter++);
	printf("%7ld ++",*(unsigned long *)sta);
	printf("%7d ++",*(int *)(sta+8));
	printf("%7d ++",*(int *)(sta+12));
	printf("%7d ++",*(int *)(sta+16));
	printf("%7d ++",*(int *)(sta+20));
	printf("%7d ++",*(int *)(sta+24));
	printf("\n");
//	fflush(stdin);
//	getchar();
	usleep(1000);
}

class Configuration {
public:
#ifndef SINGLE_NODE_TEST
	Configuration();
	virtual ~Configuration();

	static Configuration *getInstance() {
		if(configuration_==0) {
			configuration_=new Configuration();
			return configuration_;
		}
		else {
			return configuration_;
		}
	}
#endif

#ifdef SINGLE_NODE_TEST
	Configuration(const char *file){
		const char *config_file=file;
		try {
			cfg_.readFile(config_file);
		}
		catch(libconfig::FileIOException &e) {
			e.what();
		}

		initilize();
	};
	virtual ~Configuration(){};

	static Configuration *getInstance(const char * file=0) {
		if(configuration_==0) {
			configuration_=new Configuration(file);
			return configuration_;
		}
		else {
			return configuration_;
		}
	}
#endif

	bool initilize();

	string read_coordinator_ip();
	string get_coordinator_ip();

	string read_worker_ip();
	string get_worker_ip();

	int read_theron_worker_port();
	int get_theron_worker_port();

private:
	static Configuration *configuration_;

	/*
	 * TODO: add the set and get method.
	 * */
	unsigned block_size_;
	unsigned heap_size_;
	unsigned heap_sort_buffer_init_size_;

	string worker_ip_;
	string coordinator_ip_;
	/* for test on a single node. */
	int theron_worker_port_;

private:
	libconfig::Config cfg_;
};

#endif /* M_CONFIGURATION_H_ */
