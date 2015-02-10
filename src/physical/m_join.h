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
#include "../common/m_logging.h"
#include "../common/m_schema.h"
#include "../common/m_timer.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class HashJoin: public QueryPlan {
public:
	HashJoin();
	virtual ~HashJoin();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput(){};

	vector<Expression *> output(){
		vector<Expression *> ret;
		return ret;
	};

private:
	void build();
	void probe();

private:
	QueryPlan *left_;
	QueryPlan *right_;

	JoinType join_type_;

};

class MergeJoinSerObj {
public:
	MergeJoinSerObj(NewSchema, NewSchema, NewSchema, QueryPlan *, QueryPlan *);
	virtual ~MergeJoinSerObj();

	MergeJoinSerObj(){};

	NewSchema output_schema_;
	NewSchema right_schema_;
	NewSchema left_schema_;

	QueryPlan *left_;
	QueryPlan *right_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & output_schema_ & right_schema_ & left_schema_ & left_ & right_;
	}
};

class MergeJoin: public QueryPlan {
public:
	MergeJoin(vector<Expression *> leftKeys,
				  vector<Expression *> rightKeys,
				  vector<Expression *> conditions,
				  QueryPlan *left,
				  QueryPlan *right,
				  JoinType join_type);
	MergeJoin(MergeJoinSerObj *mjso)
	:merge_join_ser_obj_(mjso){}
	virtual ~MergeJoin();

	MergeJoin(){};

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput();

	vector<Expression *> output();

private:
	int compare(void *, void *);
	bool combine(void *&, void *, void *);
	static void* gather_left(void *);
	static void* gather_right(void *);
	unsigned tablesize_left;
	unsigned tablesize_right;

private:
	/* these can be omitted in this experiments. */
	vector<Expression *> left_keys_;
	vector<Expression *> right_keys_;
	vector<Expression *> conditions_;

	/* two side left and right child. */
	QueryPlan *left_;
	QueryPlan *right_;

	JoinType join_type_;

	/* it's the schema of the left and right. */
	Schema *left_schema_;
	Schema *right_schema_;
	Schema *output_schema_;

	/* this is the temp blocks which can store the block the pipeline generate. */
	Block *left_block_;
	Block *right_block_;

	/* two blocks which will store the whole data. */
	FlexBlock *left_flex_block_;
	FlexBlock *right_flex_block_;

	BufferIterator *lb_itr_;
	BufferIterator *rb_itr_;

	BufferIterator *lfb_itr_;
	BufferIterator *rfb_itr_;

	unsigned count_;
	bool over_;

private:
	MergeJoinSerObj *merge_join_ser_obj_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<QueryPlan>(*this)
				& merge_join_ser_obj_;
	}

};

class NestLoopJoinSerObj {
public:
	NestLoopJoinSerObj(NewSchema, NewSchema, NewSchema, QueryPlan *, QueryPlan *);
	virtual ~NestLoopJoinSerObj();

	NestLoopJoinSerObj(){};

	NewSchema output_schema_;
	NewSchema right_schema_;
	NewSchema left_schema_;

	QueryPlan *left_;
	QueryPlan *right_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & output_schema_ & right_schema_ & left_schema_ & left_ & right_;
	}
};

class NestLoopJoin: public QueryPlan {
public:
	NestLoopJoin(NestLoopJoinSerObj *nljso)
	:nestloop_ser_obj_(nljso){};
	NestLoopJoin();
	virtual ~NestLoopJoin();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput();

	vector<Expression *> output(){
		vector<Expression *> ret;
		return ret;
	};

private:
	/* two side left and right child. */
	QueryPlan *left_;
	QueryPlan *right_;

	JoinType join_type_;

	/* it's the schema of the left and right. */
	Schema *left_schema_;
	Schema *right_schema_;

	/* this is the temp blocks which can store the block the pipeline generate. */
	Block *left_block_;
	Block *right_block_;

	/* two blocks which will store the whole data. */
	FlexBlock *left_flex_block_;

	BufferIterator *lb_itr_;
	BufferIterator *rb_itr_;

	BufferIterator *lfb_itr_;

private:
	NestLoopJoinSerObj *nestloop_ser_obj_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<QueryPlan>(*this)
				& nestloop_ser_obj_;
	}
};

} /* namespace physical */
#endif /* M_JOIN_H_ */
