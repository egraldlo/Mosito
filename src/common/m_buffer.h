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
#include <vector>
#include <iostream>
using namespace std;

#include "m_configuration.h"

class BufferIterator;

class Buffer {
	friend class BufferIterator;
public:
	Buffer(unsigned size);
	virtual ~Buffer();

	inline void* getAddr() {
		return start_;
	}

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
	Block(unsigned size);
	virtual ~Block();

	BufferIterator *createIterator();

	unsigned getActualSize();

	void *allocateTuple();

	/* ugly way, update the free of the block. */
	void updateFree();
	/* build a block which is not full. */
	void build(int, int);
	/* get tuples */
	inline unsigned get_size() {
		return *(int *)(start_+size_-4);
	}

	/* ugly! ugly! ugly! */
	/* get the first tuple */
	int compare_start_end(vector<int>);

	bool storeTuple(void *, void *);

	bool reset();

	/* the data buffer which has no tuple number. */
	bool assembling(int size, int tuple_size);

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

	bool storeTupleOK(void *);

	bool reset();

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

	/* for merged sort in the pcbuffer. */
	void *getCurrent();
	void increaseCur();

	void *getNextFake();

	void reset();

public:
	int get_size();

private:
	Buffer *buffer_;
	unsigned current_;
	unsigned fake_count_;
};

#endif /* M_BUFFER_H_ */
