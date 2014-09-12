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

#include <vector>
using namespace std;

class Expression {
public:
	Expression();
	virtual ~Expression();

	virtual void* eval()=0;
	virtual void initilize()=0;

public:
	data_type return_type;
	/* references will be a vector, because it can be presented as vector<column>. */
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

#endif /* M_EXPRESSION_H_ */
