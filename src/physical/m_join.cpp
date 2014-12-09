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

MergeJoin::MergeJoin(
		  vector<Expression *> leftKeys,
		  vector<Expression *> rightKeys,
		  vector<Expression *> conditions,
		  QueryPlan *left,
		  QueryPlan *right,
		  JoinType join_type)
:left_keys_(leftKeys),right_keys_(rightKeys),conditions_(conditions),
 left_(left),right_(right),join_type_(join_type){

}

MergeJoin::~MergeJoin() {

}

/*
 * we collect all data here, before this we will merge them in the shufflelower.
 *  */
bool MergeJoin::prelude() {
	left_->prelude();
	right_->prelude();

	left_schema_=new Schema(&(left_->output()));
	right_schema_=new Schema(&(right_->output()));

	unsigned left_tuple_size_=left_schema_->get_bytes();
	unsigned right_tuple_size_=right_schema_->get_bytes();

	left_block_=new Block(BLOCK_SIZE,left_tuple_size_);
	right_block_=new Block(BLOCK_SIZE,right_tuple_size_);

	/*
	 * here we use the universal solution, and we store all data in memory.
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

	lb_itr_=left_flex_block_->createIterator();
	rb_itr_=right_flex_block_->createIterator();

	return true;
}

bool MergeJoin::execute(Block *block) {
	/* merge the two tables and output block. */
	void *desc=0;
	void *l=lb_itr_->getNext();
	void *r=rb_itr_->getNext();
	void *fake_l=0;
	void *fake_r=0;
	while(l&&r) {
		if(compare(l, r)==1) {
			if((desc=block->allocateTuple())!=0) {
				combine(desc, l, r);
			}
			else {
				return true;
			}
			fake_l=lb_itr_->getNextFake();
			fake_r=rb_itr_->getNextFake();
			while(fake_l!=0) {
				if(compare(l, fake_r)==1) {
					if((desc=block->allocateTuple())!=0) {
						combine(desc, fake_l, r);
					}
					else {
						return true;
					}
					fake_l=lb_itr_->getNextFake();
				}
				else {
					break;
				}
			}
			while(fake_r!=0) {
				if(compare(l, fake_r)==1) {
					if((desc=block->allocateTuple())!=0) {
						combine(desc, l, fake_r);
					}
					else {
						return true;
					}
					fake_r=rb_itr_->getNextFake();
				}
				else {
					break;
				}
			}
			l=lb_itr_->getNext();
			r=rb_itr_->getNext();
		}
		else if(compare(l, r)==0){
			l=lb_itr_->getNext();
		}
		else {
			r=rb_itr_->getNext();
		}
	}

	return false;
}

int MergeJoin::compare(void *left, void *right) {
	/*
	 * by using '=' expression and filter out the equal expression
	 * todo: a binding strategy to bind the 'conditions_' to child's input
	 * left_keys_ and right_keys_,
	 * */
	if(*(int *)left==*(int *)right)
		return 1;
	if(*(int *)left<*(int *)right)
		return 0;
	if(*(int *)left>*(int *)right)
		return -1;
}

bool MergeJoin::combine(void *des, void *left, void *right) {
	memcpy(des, left, left_schema_->get_bytes());
	memcpy(des+left_schema_->get_bytes(), right, right_schema_->get_bytes());
	return true;
}

bool MergeJoin::postlude() {
	return true;
}

vector<Expression *> MergeJoin::output() {
	return left_->output();
}

NestLoopJoin::NestLoopJoin() {

}

NestLoopJoin::~NestLoopJoin() {

}

} /* namespace physical */
