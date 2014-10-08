/*
 * m_data_type.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_DATA_TYPE_H_
#define M_DATA_TYPE_H_

enum data_type{t_int,t_double,t_long};

class DataType {
public:
	DataType();
	virtual ~DataType();

	virtual bool gt(void *, void *)=0;
	virtual bool ge(void *, void *)=0;
	virtual bool eq(void *, void *)=0;
	virtual bool ls(void *, void *)=0;
	virtual bool le(void *, void *)=0;
};

class IntegerType: public DataType {
public:
	IntegerType();
	virtual ~IntegerType();

	inline bool gt(void *l, void *r) {
		return *(int *)l>*(int *)r;
	}

	inline bool ge(void *l, void *r) {
		return *(int *)l>=*(int *)r;
	}

	inline bool eq(void *l, void *r) {
		return *(int *)l==*(int *)r;
	}

	inline bool ls(void *l, void *r) {
		return *(int *)l<*(int *)r;
	}

	inline bool le(void *l, void *r) {
		return *(int *)l<=*(int *)r;
	}
};

#endif /* M_DATA_TYPE_H_ */
