/*
 * m_configuration.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_CONFIGURATION_H_
#define M_CONFIGURATION_H_

/* provide INT_MIN, INT_MAX etc. */
#include <limits.h>

/* block size of pipeline. */
#define BLOCK_SIZE 64*1024
/* this is the heap size to sort the stream. */
#define HEAP_SIZE 100
/* buffer the data which is from the heap. */
#define HEAP_SORT_BUFFER_INIT_SIZE 16
#define HEAP_SORT_BUFFER_INCREASING_FACTOR 2

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
