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
	for(int i=0; i<row; i++) {
		data_[i]=new BlockCircleQueue(COLUMN_CON);
	}
}

PCBuffer::~PCBuffer() {
	// TODO Auto-generated destructor stub
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

	while((block=data_[column]->pop())==0);
	Logging::getInstance()->log(trace, "get a block from the pcbuffer.");
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
