/*
 * m_partitioner.h
 *
 *  Created on: 2014-11-10
 *      Author: casa
 */

#ifndef M_PARTITIONER_H_
#define M_PARTITIONER_H_

class Partitioner {
public:
	Partitioner();
	virtual ~Partitioner();

	virtual int partition(void *)=0;
};

class HashPartitioner: public Partitioner {
public:
	HashPartitioner();
	~HashPartitioner();

	int partition(void *){
		return 0;
	};
};

class RangePartitioner: public Partitioner {
public:
	RangePartitioner();
	~RangePartitioner();

	int partition(void *){
		return 0;
	};
};

#endif /* M_PARTITIONER_H_ */
