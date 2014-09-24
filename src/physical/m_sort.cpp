/*
 * m_sort.cpp
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#include "m_sort.h"

namespace physical {

Sort::Sort(vector<SortOrder *> expressions, QueryPlan *child, bool global)
:expressions_(expressions),child_(child),global_(global) {

}

Sort::~Sort() {

}

bool Sort::prelude() {
	child_->prelude();
	/* TODO: output_ must be compute. */
	vector<Expression *> input_=child_->output();
	schema_=new Schema(&input_);
	heap_=new Heap(HEAP_SORT_BUFFER_INIT_SIZE,schema_->get_bytes());
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
	BufferIterator *bi=0;

	void *tuple=0;
	if(child_->execute(buffer_)) {
		bi=buffer_->createIterator();
	}
	else {
		/* TODO: the same as the project iterator */
	}

	if(heap_->init_phase(bi)) {
		/* if true, we sort the array_ by using heap sort. */
		heap_->heap_sort();
		while((tuple==bi->getNext())!=0) {

		}
	}
	else {
		/* if false, we sort the array_ also, but we use waterline. */
		heap_->heap_sort();

	}

	while(child_->execute(buffer_)) {
		bi=buffer_->createIterator();
	}
	buffer_->createIterator();
	return true;
}

bool Sort::execute(Block *block) {

	return true;
}

bool Sort::postlude() {
	child_->postlude();
	return true;
}

vector<Expression *> Sort::output() {
	return child_->output();
}

}
