/*
 * m_data_type.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_DATA_TYPE_H_
#define M_DATA_TYPE_H_

enum data_type{t_int,t_double};

class DataType {
public:
	DataType();
	virtual ~DataType();
};

class Column {
public:
	Column();
	virtual ~Column();
};

class IntegerColumn: public Column {
public:
	IntegerColumn();
	virtual ~IntegerColumn();
};

#endif /* M_DATA_TYPE_H_ */
