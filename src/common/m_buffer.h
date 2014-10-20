/*
 * m_buffer.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_BUFFER_H_
#define M_BUFFER_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
using namespace std;

#include "m_configuration.h"

class BufferIterator;

class Buffer {
	friend class BufferIterator;
public:
	Buffer(unsigned size);
	virtual ~Buffer();

	virtual BufferIterator *createIterator()=0;

	virtual unsigned getActualSize()=0;

	virtual void *allocateTuple()=0;

	virtual bool storeTuple(void *, void *)=0;

	virtual bool reset()=0;

protected:
	char *start_;
	char *free_;
	unsigned size_;
};

class Block: public Buffer {
public:
	Block(unsigned size, unsigned tuple_size);
	virtual ~Block();

	BufferIterator *createIterator();

	unsigned getActualSize();

	void *allocateTuple();

	bool storeTuple(void *, void *);

	bool reset();

public:
	/* can be used to serialization */
	bool storeBlock(void *, unsigned);

private:
	unsigned tuple_size_;
};

/*
 * FlexBlock can be resized by 2 times.
 * */
class FlexBlock: public Block {
public:
	FlexBlock(unsigned size, unsigned tuple_size, double increasing_factor=2);
	virtual ~FlexBlock();

	bool double_buffer();

protected:
	double increasing_factor_;
};

class PersistFlexBlock: public FlexBlock {
public:
	PersistFlexBlock(unsigned size, unsigned tuple_size, double increasing_factor=2);
	virtual ~PersistFlexBlock();

	bool persist(string );
};

class BufferIterator {
public:
	BufferIterator(Buffer *buffer);
	virtual ~BufferIterator();

	void* getNext();

public:
	inline int get_size() {
		/* "4" is the tail length.
		 * TODO: tail info can be more rich.
		 *  */
		return *(int *)(buffer_->start_+buffer_->size_-4);
	};

private:
	Buffer *buffer_;
	unsigned current_;
};

#endif /* M_BUFFER_H_ */
