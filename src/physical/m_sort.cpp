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

	vector<SortOrderRef *> sor;
	for(unsigned i=0;i<expressions_.size();i++) {
		for(unsigned j=0;j<input_.size();j++) {
			if(expressions_[i]->expression_->expr_id==input_[j]->expr_id)
				/* TODO: to do some switch. */
				sor.push_back(new SortOrderRef(1,expressions_[i]->direction_));
		}
	}

	Comparator *compr=new Comparator(schema_, sor);

	lt_=new LoserTree(compr);
	heap_=new Heap(HEAP_SORT_BUFFER_INIT_SIZE, schema_->get_bytes(), compr);
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());

	void *tuple=0;

	while(child_->execute(buffer_)) {
		BufferIterator bi=*buffer_->createIterator();
		while((tuple=bi.getNext())!=0) {
			if(!heap_->heap_empty()) {
				if(heap_->init_phase(bi)) {
					heap_->heap_sort();
				}
				else {
					heap_->heap_sort();
					/* TODO: return directly. */
				}
			}
			heap_->heap_adjust(tuple);
		}
	}
	cout<<"hello?"<<endl;
	heap_->cleanup();

	vector<string> files=heap_->get_files();
	/* loser tree buffer to merge the sequential data. */
	unsigned files_no=files.size();
	lt_buffer_=new Block*[files_no];
	lt_buffer_iterator_=new BufferIterator*[files_no];
	for(unsigned i=0;i<files_no;i++) {

		lt_buffer_[i]=new Block(LOOSE_TREE_BUFFER_SIZE, schema_->get_bytes());
		lt_buffer_iterator_[i]=lt_buffer_[i]->createIterator();
	}
	return true;
}

bool Sort::execute(Block *block) {
	void *tuple=0;
	while((tuple=block->allocateTuple())!=0) {

	}
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
