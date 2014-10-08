/*
 * m_join.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_join.h"

namespace physical {

Join::Join() {

}

Join::~Join() {

}

HashJoin::HashJoin() {

}

HashJoin::~HashJoin() {

}

SortMergeJoin::SortMergeJoin(
		  vector<Expression *> leftKeys,
		  vector<Expression *> rightKeys,
		  vector<Expression *> conditions,
		  QueryPlan *left,
		  QueryPlan *right,
		  JoinType join_type)
:left_keys_(leftKeys),right_keys_(rightKeys),conditions_(conditions),
 left_(left),right_(right),join_type_(join_type){

}

SortMergeJoin::~SortMergeJoin() {

}

bool SortMergeJoin::prelude() {
	left_->prelude();
	right_->prelude();

	left_schema_=new Schema(&(left_->output()));
	right_schema_=new Schema(&(right_->output()));

	unsigned left_tuple_size_=left_schema_->get_bytes();
	unsigned right_tuple_size_=right_schema_->get_bytes();

	left_flex_block_=new FlexBlock(BLOCK_SIZE,left_tuple_size_);
	right_flex_block_=new FlexBlock(BLOCK_SIZE,right_tuple_size_);



	return true;
}

bool SortMergeJoin::execute(Block *block) {

	return true;
}

bool SortMergeJoin::postlude() {
	return true;
}

vector<Expression *> SortMergeJoin::output() {
	return left_->output();
}

NestLoopJoin::NestLoopJoin() {

}

NestLoopJoin::~NestLoopJoin() {

}

} /* namespace physical */
