/*
 * m_pc_buffer.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Casa
 */

#ifndef M_PC_BUFFER_H_
#define M_PC_BUFFER_H_

#include "../expressions/m_expression.h"
#include "../common/m_circle_queue.h"
#include "../common/m_logging.h"
#include "m_buffer.h"

/*
 * the pc buffer is a producer-consumer buffer, the buffer is a two-dimensional
 * block array, it store all the blocks which are from one node in a column, if
 * the partition strategy is range partition, all the column must be merged
 * sort, so a merge sort function will be added in.
 *  */
class PCBuffer {
public:
	PCBuffer(NewSchema &, int);
	virtual ~PCBuffer();

	NewSchema getSchema();

	Block **merge_sort();

	bool put(Block *, int);
	bool get(Block *&, int);
	bool get_sorted(Block *&);

private:
//	CircleQueue<Block> **data_;
	BlockCircleQueue **data_;
	NewSchema ns_;
	int row_;

private:
	Block ** merged_blocks_;
	BufferIterator **itrs_;
	bool begin_;
};

class SortedPCBuffer {
public:
	SortedPCBuffer(NewSchema & ns, int row){}
	virtual ~SortedPCBuffer(){}

	FlexBlock **flex_block_;

public:
	NewSchema getSchema();

};

#endif /* M_PC_BUFFER_H_ */
