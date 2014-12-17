/*
 * m_shuffle_upper.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_upper.h"

namespace physical {

ShuffleUpper::ShuffleUpper(vector<Expression *> expressions, QueryPlan *child) {

}

ShuffleUpper::ShuffleUpper(Merger * merger) {
	merger_=merger;
}

ShuffleUpper::~ShuffleUpper() {

}

bool ShuffleUpper::prelude() {
	/* send the task into shuffle lower. */

	/* here we must create pthread to receive the data, it's a producer.*/
	return true;
}

bool ShuffleUpper::execute(Block *block) {
	/* it's a consumer, if the buffer has blocks and pipeline it the upper operator. */

	return true;
}

bool ShuffleUpper::postlude() {
	return true;
}

bool ShuffleUpper::serialization() {
	for(int i=0; i<2; i++) {

	}
	return true;
}

}
