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
		  JoinType join_type) {

}

SortMergeJoin::~SortMergeJoin() {

}

bool SortMergeJoin::prelude() {

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
