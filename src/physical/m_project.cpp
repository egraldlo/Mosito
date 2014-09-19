/*
 * m_project.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_project.h"
#include <stdio.h>

namespace physical {

Project::Project(vector<Expression *> expressions, QueryPlan *child)
:expressions_(expressions), child_(child), buffer_(0), buffer_iterator_(0) {

}

Project::~Project() {

}

bool Project::prelude() {
	child_->prelude();
	/* TODO: output_ must be compute. */
	vector<Expression *> input_=child_->output();
	schema_=new Schema(&input_);
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
	return true;
}

bool Project::execute(Block *block) {
	void *desc;
	void *src;
	block->reset();
	while((desc=block->allocateTuple())!=0) {
		if(buffer_iterator_==0) {
			if(child_->execute(buffer_)) {
				buffer_iterator_=buffer_->createIterator();
			}
		}
		if((src=buffer_iterator_->getNext())!=0) {
			/* we can at here print the tuple, use the src
			 * TODO: construct a Row(which will be constructed) with the expressions.
			 * */
			block->storeTuple(desc,src);
		}
		else{
			if(child_->execute(buffer_)) {
				buffer_iterator_=buffer_->createIterator();
				src=buffer_iterator_->getNext();
				/* we can at here print the tuple, use the src
				 * TODO: construct a Row(which will be constructed) with the expressions.
				 * */
				block->storeTuple(desc,src);
			}
			else {
				return false;
			}
		}
	}
	return true;
}

bool Project::postlude() {
	child_->postlude();
	return true;
}

vector<Expression *> Project::output() {
	return child_->output();
}

}
