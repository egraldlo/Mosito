/*
 * m_arithmetic.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_ARITHMETIC_H_
#define M_ARITHMETIC_H_

#include <stdlib.h>

#include "../common/m_data_type.h"

#include "m_expression.h"
#include "m_type_cast.h"

typedef void* (*AddFunc)(void *,void *);

class Literal: public LeafExpression {
public:
	Literal(){};
	virtual ~Literal(){};

	virtual void initilize(){val=malloc(8);};
	void* eval(){return val;};

};

class ADD: public BinaryExpression {
public:
	ADD(Expression *left, Expression *right);
	virtual ~ADD();

	virtual void initilize();
	void* eval();

public:
	Expression *left,*right;
	AddFunc add[2];
};

#endif /* M_ARITHMETIC_H_ */
