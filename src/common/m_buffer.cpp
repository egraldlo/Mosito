/*
 * m_buffer.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_buffer.h"

#include <iostream>

BufferIterator::BufferIterator(Buffer *buffer)
:buffer_(buffer), current_(0) {

}

BufferIterator::~BufferIterator() {

}

void *BufferIterator::getNext() {
	unsigned tuple_size=buffer_->getActualSize();
	void *ret=buffer_->start_+current_*tuple_size;
	/* "ret<buffer_->free_" is wrong, we must use tuple count.*/
	cout<<"get_size(): "<<get_size()<<endl;
	if(get_size()==0)
		getchar();
	if(++current_==get_size()) {
		cout<<"return 0"<<endl;
		return 0;
	}
	else{
		return ret;
	}
}

Buffer::Buffer(unsigned size)
:size_(size),start_(0),free_(0){
	start_=(char *)malloc(size_);
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
	size_=HEAP_SORT_BUFFER_INIT_SIZE;
	free_=start_;
	return true;
}

bool Block::storeBlock(void *src, unsigned size) {
	memcpy(start_,src,size);
	free_=start_+size;
	return true;
}

FlexBlock::FlexBlock(unsigned size, unsigned tuple_size, double increasing_factor)
:Block(size, tuple_size),increasing_factor_(increasing_factor){

}

FlexBlock::~FlexBlock() {

}

bool FlexBlock::double_buffer() {
	unsigned used=free_-start_;
	size_=size_*increasing_factor_;
	start_=(char *)realloc(start_,size_);
	free_=start_+used;
	/* TODO: add realloc error solution. */
	return true;
}

PersistFlexBlock::PersistFlexBlock(unsigned size, unsigned tuple_size, double increasing_factor)
:FlexBlock(size,tuple_size,increasing_factor){

}

PersistFlexBlock::~PersistFlexBlock() {

}

bool PersistFlexBlock::persist(string filename) {
	FILE *file=fopen(filename.c_str(),"wb+");
	fwrite(start_,free_-start_,1,file);
	fclose(file);
	return true;
}
