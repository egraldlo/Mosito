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

bool HashJoin::prelude() {

}

bool HashJoin::execute(Block *) {

}

bool HashJoin::postlude() {

}

void HashJoin::partition() {
	
}

void HashJoin::build() {

}

void HashJoin::probe() {

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

	left_block_=new Block(BLOCK_SIZE,left_tuple_size_);
	right_block_=new Block(BLOCK_SIZE,right_tuple_size_);

	/* here we use the universal solution, and we store all data in memory.
	 * it's not a good solution because it will waste a lot of memory space.
	 *  */
	left_flex_block_=new FlexBlock(INIT_FLEX_BLOCK_SIZE,left_tuple_size_);
	right_flex_block_=new FlexBlock(INIT_FLEX_BLOCK_SIZE,right_tuple_size_);

	void *ltuple,*rtuple;

	/* collect the left stream data. */
	while(!left_->execute(left_block_)) {
		lb_itr_=left_block_->createIterator();
		lb_itr_->reset();
		while((ltuple=lb_itr_->getNext())!=0) {
			left_flex_block_->storeTupleOK(ltuple);
		}
	}

	/* collect the right stream data. */
	while(!right_->execute(right_block_)) {
		rb_itr_=right_block_->createIterator();
		rb_itr_->reset();
		while((rtuple=rb_itr_->getNext())!=0) {
			right_flex_block_->storeTupleOK(rtuple);
		}
	}

	return true;
}

bool SortMergeJoin::execute(Block *block) {
	/* merge the two tables and output block. */
	void *desc=0;
	while((desc=block->allocateTuple())!=0) {

	}

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
