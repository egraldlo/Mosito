/*
 * m_join.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_JOIN_H_
#define M_JOIN_H_

enum JoinType{inner, left, right, full};

#include "../../src/expressions/m_expression.h"
#include "../../src/common/m_buffer.h"
#include "../common/m_configuration.h"
#include "../common/m_tree_node.h"
#include "../common/m_schema.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class Join: public BinaryNode, public QueryPlan {
public:
	Join();
	virtual ~Join();
};

class HashJoin: public Join {
public:
	HashJoin();
	virtual ~HashJoin();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output(){
		vector<Expression *> ret;
		return ret;
	};

private:
	void partition();
	void build();
	void probe();

private:
	QueryPlan *left_;
	QueryPlan *right_;

	JoinType join_type_;

};

class SortMergeJoin: public Join {
public:
	SortMergeJoin(vector<Expression *> leftKeys,
				  vector<Expression *> rightKeys,
				  vector<Expression *> conditions,
				  QueryPlan *left,
				  QueryPlan *right,
				  JoinType join_type);
	virtual ~SortMergeJoin();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output();

private:
	vector<Expression *> left_keys_;
	vector<Expression *> right_keys_;
	vector<Expression *> conditions_;

	QueryPlan *left_;
	QueryPlan *right_;

	JoinType join_type_;

	Schema *left_schema_;
	Schema *right_schema_;

	Block *left_block_;
	Block *right_block_;

	FlexBlock *left_flex_block_;
	FlexBlock *right_flex_block_;

	BufferIterator *lb_itr_;
	BufferIterator *rb_itr_;

	BufferIterator *lfb_itr_;
	BufferIterator *rfb_itr_;
};

class NestLoopJoin: public Join {
public:
	NestLoopJoin();
	virtual ~NestLoopJoin();

	bool prelude(){return true;};
	bool execute(Block *){return true;};
	bool postlude(){return true;};

	vector<Expression *> output(){
		vector<Expression *> ret;
		return ret;
	};

private:

};

} /* namespace physical */
#endif /* M_JOIN_H_ */
