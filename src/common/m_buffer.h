/*
 * m_buffer.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_BUFFER_H_
#define M_BUFFER_H_

#include <string.h>

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

public:
	/* can be used to serialization */
	bool storeBlock(void *, unsigned);

private:
	unsigned tuple_size_;
};

class BufferIterator {
public:
	BufferIterator(Buffer *buffer);
	virtual ~BufferIterator();

	void* getNext();

private:
	Buffer *buffer_;
	unsigned current_;
};

#endif /* M_BUFFER_H_ */
