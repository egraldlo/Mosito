/*
 * m_shuffle_lower.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_lower.h"

namespace physical {

ShuffleLowerSerObj::ShuffleLowerSerObj(NewSchema ns, vector<int> seqs, QueryPlan* child)
:ns_(ns), seqs_(seqs), child_(child){

}

ShuffleLowerSerObj::~ShuffleLowerSerObj() {

}

ShuffleLower::ShuffleLower(vector<Expression *> expressions, QueryPlan *child) {

}

ShuffleLower::ShuffleLower(Sender** senders) {
	senders_=senders;
}

ShuffleLower::~ShuffleLower() {

}

bool ShuffleLower::prelude() {
	/* create a thread to send the block into the upper. */

	return true;
}

bool ShuffleLower::execute(Block *block) {
	/* get the block from the pipeline. */

	return true;
}

bool ShuffleLower::postlude() {

	return true;
}

}
