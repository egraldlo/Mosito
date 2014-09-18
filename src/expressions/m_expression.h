/*
 * m_expression.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_EXPRESSION_H_
#define M_EXPRESSION_H_

#include "../common/m_data_type.h"
#include "../common/m_tree_node.h"

#include <stdlib.h>

#include <vector>
#include <iostream>
using namespace std;

class Column;

class Expression {
public:
	Expression();
	virtual ~Expression();

	virtual void* eval()=0;
	virtual void initilize()=0;
	virtual void display();

public:
//protected:
	data_type return_type;
	/*
	 * references will be a vector, because it can be presented as vector<column>.
	 * if "a+2" a belongs the references but 2 is not.
	 *  */
	vector<Column *> references;
	void *val;
};

class UnaryExpression: public Expression, public UnaryNode {
public:
	UnaryExpression(){};
	virtual ~UnaryExpression(){};

	virtual void initilize(){};
};

class BinaryExpression: public Expression, public BinaryNode {
public:
	BinaryExpression(){};
	virtual ~BinaryExpression(){};

	virtual void initilize(){};
};

class LeafExpression: public Expression, public LeafNode {
public:
	LeafExpression(){};
	virtual ~LeafExpression(){};

	virtual void initilize(){};
};

class Literal: public LeafExpression {
public:
	Literal(){};
	virtual ~Literal(){};

	virtual void initilize(){val=malloc(8);};
	void* eval(){return val;};
};

class Column: public LeafExpression {
public:
	Column(){};
	virtual ~Column(){};

	void *eval(){};
	void initilize(){};
};

#endif /* M_EXPRESSION_H_ */
