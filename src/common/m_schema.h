/*
 * m_schema.h
 *
 *  Created on: 2014-9-18
 *      Author: casa
 */

#ifndef M_SCHEMA_H_
#define M_SCHEMA_H_

#include "../common/m_data_type.h"
#include "../../src/expressions/m_expression.h"

#include <vector>
using namespace std;

class Schema {
public:
	Schema();
	Schema(vector<Expression*> *);

#ifdef EXPERIMENT
	Schema(NewSchema *ns);
#endif

	virtual ~Schema();

	/* get a data type from the v_d_. */
	inline DataType* getDataType(int off) {
		return v_d_[off];
	}

	/* get the seq-th column from the tuple which has the address start. */
	inline void *get_addr(void *start, int seq) {
		return (char *)start+offset_[seq];
	}

	int get_bytes();

	int get_columns();
private:
	/* vector of data type pointer. */
	vector<DataType*> v_d_;
	/* offset of every column. */
	int* offset_;
	/* tatolsize of the schema. */
	int bytes_;
};

#endif /* M_SCHEMA_H_ */
