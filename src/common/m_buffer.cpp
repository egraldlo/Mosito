/*
 * m_buffer.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_buffer.h"

BufferIterator::BufferIterator(Buffer *buffer)
:buffer_(buffer), current_(0) {

}

BufferIterator::~BufferIterator() {

}

void *BufferIterator::getNext() {
	unsigned tuple_size=buffer_->getActualSize();
	void *ret=buffer_->start_+(current_++)*tuple_size;
	/* "ret<buffer_->free_" is wrong, we must use tuple count.*/
	if(current_==get_size()) {
		return 0;
	}
	else{
		return ret;
	}
}

Buffer::Buffer(unsigned size)
:size_(size),start_(0),free_(0){
	start_=new char[size_];
	free_=start_;
}

Buffer::~Buffer() {

}

Block::Block(unsigned size, unsigned tuple_size)
:Buffer(size),tuple_size_(tuple_size){

}

Block::~Block() {

}

BufferIterator *Block::createIterator() {
	return new BufferIterator(this);
}

unsigned Block::getActualSize() {
	return tuple_size_;
}

void *Block::allocateTuple() {
	if(free_+tuple_size_<=start_+size_)
		return free_;
	else
		return 0;
}

bool Block::storeTuple(void *desc, void *src) {
	memcpy(desc,src,tuple_size_);
	free_+=tuple_size_;
	return true;
}

bool Block::reset() {
	free_=start_;
	return true;
}

bool Block::storeBlock(void *src, unsigned size) {
	memcpy(start_,src,size);
	free_=start_+size;
	return true;
}

FlexBlock::FlexBlock(unsigned size, unsigned tuple_size)
:Block(size, tuple_size){

}

FlexBlock::~FlexBlock() {

}

bool FlexBlock::double_buffer() {
	unsigned used=size_;
	size_*=increasing_factor_;
	start_=(char *)realloc(start_,size_);
	free_=start_+used;
	/* TODO: add realloc error solution. */
	return true;
}
