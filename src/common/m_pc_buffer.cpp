/*
 * m_pc_buffer.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Casa
 */

#include "m_pc_buffer.h"

PCBuffer::PCBuffer(NewSchema &ns, int row)
:ns_(ns), row_(row) {
	// TODO Auto-generated constructor stub
	data_=new BlockCircleQueue*[row];
	merged_blocks_=new Block*[row];
	itrs_=new BufferIterator*[row];
	for(int i=0; i<row; i++) {
		data_[i]=new BlockCircleQueue(COLUMN_CON);
		merged_blocks_[i]=new Block(BLOCK_SIZE, ns.get_bytes());
	}
	begin_=false;
}

PCBuffer::~PCBuffer() {
	// TODO Auto-generated destructor stub
	for(int i=0; i<row_; i++) {
		data_[i]->~BlockCircleQueue();
	}
}

bool PCBuffer::get(Block* &block, int column) {
	/* 20150130
	if(data_[column]->empty()) {
		return false;
	}
	else {
		block=data_[column]->pop();
		Logging::getInstance()->log(trace, "get a block from the pcbuffer.");
		return true;
	}*/

	int seconds_=0;
	while((block=data_[column]->pop())==0) {
		/* waiting for a while and then return false. */
		if(seconds_++==10) return false;
	};
	Logging::getInstance()->log(trace, "get a block from the pcbuffer.");
	return true;
}

bool PCBuffer::get_sorted(Block *&block) {
	if(!begin_) {
		for(int i=0; i<row_; i++) {
			merged_blocks_[i]->storeBlock(data_[i]->pop()->getAddr(), BLOCK_SIZE);
			itrs_[i]=merged_blocks_[i]->createIterator();
		}
		begin_=true;
	}

	void *desc=0;
	while((desc=block->allocateTuple())!=0) {
		void *tuple=0;
		void *most=0;
		int most_cur=0;
		for(int i=0; i<row_; i++) {
			if(!itrs_[i]->getCurrent()==0) {
				if(data_[i]->pop()->get_size()==0) {

				}
			}
		}
		/* choose the needed one tuple. */
		block->storeTuple(desc, tuple);
	}

	/* build a 0-size one block. */
	return true;
}

bool PCBuffer::put(Block *block, int column) {
	while(!data_[column]->push(block));
	Logging::getInstance()->log(trace, "put a block into the pcbuffer.");
	return true;
}

NewSchema PCBuffer::getSchema() {
	return ns_;
}
