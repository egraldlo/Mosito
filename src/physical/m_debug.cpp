/*
 * m_debug.cpp
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#include "m_debug.h"

namespace physical {

Debug::Debug(vector<Expression *> expressions, Iterator *child)
:expressions_(expressions), child_(child){
	schema_=new Schema(&expressions);
}

Debug::~Debug() {

}

bool Debug::prelude() {
	child_->prelude();
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
	return true;
}

bool Debug::execute(Block *) {
	BufferIterator *bi=0;
	void *tuple;
	while(child_->execute(buffer_)) {
		bi=buffer_->createIterator();
		while((tuple=bi->getNext())!=0) {
//			display(tuple);
			cout<<" | "<<*(unsigned long *)(schema_->get_addr(tuple,0));
			cout<<" | "<<*(int *)(schema_->get_addr(tuple,1));
			cout<<" | "<<*(int *)(schema_->get_addr(tuple,2));
			cout<<" | "<<*(int *)(schema_->get_addr(tuple,3));
			cout<<" | "<<*(int *)(schema_->get_addr(tuple,4));
			cout<<" | "<<*(int *)(schema_->get_addr(tuple,5))<<endl;
			getchar();
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
		expressions_[i]->display();
		cout<<" | "<<endl;
	}
}

}
