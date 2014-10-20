/*
 * m_configuration.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_CONFIGURATION_H_
#define M_CONFIGURATION_H_

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
#define HEAP_SIZE 800
/* buffer the data which is from the heap. */
#define HEAP_SORT_BUFFER_INIT_SIZE 1024
#define HEAP_SORT_BUFFER_INCREASING_FACTOR 2

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
//	usleep(1000000);
}

class Configuration {
public:
	Configuration();
	virtual ~Configuration();

private:
	/*
	 * TODO: add the set and get method.
	 * */
	unsigned block_size_;
	unsigned heap_size_;
	unsigned heap_sort_buffer_init_size_;
};

#endif /* M_CONFIGURATION_H_ */
