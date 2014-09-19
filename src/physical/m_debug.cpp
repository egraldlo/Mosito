/*
 * m_debug.cpp
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#include "m_debug.h"

namespace physical {

Debug::Debug(vector<Expression *> expressions, QueryPlan *child)
:expressions_(expressions), child_(child) {

}

Debug::~Debug() {

}

bool Debug::prelude() {
	child_->prelude();
	/* TODO: output_ must be compute. */
	vector<Expression *> input_=child_->output();
	schema_=new Schema(&input_);
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
	return true;
}

bool Debug::execute(Block *) {
	BufferIterator *bi=0;
	void *tuple;
	while(child_->execute(buffer_)) {
		bi=buffer_->createIterator();
		while((tuple=bi->getNext())!=0) {
			display(tuple);
		}
	}
	return true;
}

bool Debug::postlude() {
	child_->postlude();
	return true;
}

void Debug::display(void *tuple) {
	for(unsigned i=0;i<expressions_.size();i++) {
		expressions_[i]->display(schema_->get_addr(tuple,i));
	}
	cout<<endl;
}

vector<Expression *> Debug::output() {
	return child_->output();
}

}
