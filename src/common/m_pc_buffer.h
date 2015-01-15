/*
 * m_pc_buffer.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Casa
 */

#ifndef M_PC_BUFFER_H_
#define M_PC_BUFFER_H_

#include "../expressions/m_expression.h"
#include "m_buffer.h"

/*
 * the pc buffer is a producer-consumer buffer, the buffer is a two-dimensional
 * block array, it store all the blocks which are from one node in a column, if
 * the partition strategy is range partition, all the column must be merged
 * sort, so a merge sort function will be added in.
 *  */
class PCBuffer {
public:
	PCBuffer(NewSchema *);
	virtual ~PCBuffer();

	NewSchema *getSchema();

	Block **merge_sort();

private:
	/* synchronized strategy must require lock and semaphore. */

private:
	Block ***data_;
	NewSchema *ns_;
};

#endif /* M_PC_BUFFER_H_ */
