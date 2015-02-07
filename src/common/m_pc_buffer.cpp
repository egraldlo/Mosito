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
	int seconds_=0;
	while((data_[column]->pop(block))==false) {
		/* waiting for a while and then return false. */
		if(seconds_++==10) return false;
	};

//	int seconds_=0;
//	while((block=data_[column]->pop())==0) {
//		/* waiting for a while and then return false. */
//		if(seconds_++==10) return false;
//	};
	Logging::getInstance()->log(trace, "get a block from the pcbuffer.");
	return true;
}

bool maxLast(void *start, NewSchema *schema) {
	char *p=(char *)malloc(schema->get_bytes());
	for(int i=0;i<schema->vd_.size();i++) {
		if(schema->vd_[i]->get_type()==t_long) {
			*(unsigned long *)(p+8*i)=ULONG_MAX;
		}
	}
	start=p;
}

bool PCBuffer::get_sorted(Block *&block) {
	if(!begin_) {
		for(int i=0; i<row_; i++) {
			while(data_[i]->pop(merged_blocks_[i])==false);
			itrs_[i]=merged_blocks_[i]->createIterator();
		}
		CompareTemp *comp=new CompareTemp();
		lt_=new LoserTree(comp);
		array_=new void*[row_+1];
		for(int i=0; i<row_; i++) {
			array_[i+1]=itrs_[i]->getNext();
		}
		lt_->initialize(array_, row_);
		begin_=true;
	}

	void *desc=0;
	void *tuple=0;
	block->reset();
	while((desc=block->allocateTuple())!=0) {
		int winner=lt_->Winner();
		block->storeTuple(tuple, lt_->Win());
		if((tuple=itrs_[winner-1]->getNext())==0) {
			merged_blocks_[winner-1]->reset();
			data_[winner-1]->pop(merged_blocks_[winner-1]);
			if(merged_blocks_[winner-1]->get_size()==0) {
				maxLast(array_[winner], &ns_);
				//if finished we can return.
				if(++finished_==row_) {
					block->build(BLOCK_SIZE, 0);
					return false;
				}
			}
			tuple=itrs_[winner-1]->getNext();
		}
		lt_->Load(winner, tuple);
		if(lt_->replay(winner)!=0)
			break;
	}

	/* build a 0-size one block. */
	return true;
}

bool PCBuffer::put(Block *block, int column) {
//	while(!data_[column]->push(block));
	int seconds_=0;
	while((data_[column]->push(block))==false) {
		/* waiting for a while and then return false. */
		if(seconds_++==10) return false;
	};
	Logging::getInstance()->log(trace, "put a block into the pcbuffer.");
	return true;
}

NewSchema PCBuffer::getSchema() {
	return ns_;
}
