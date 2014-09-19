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
	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
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
