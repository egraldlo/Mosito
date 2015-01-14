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
	Logging::getInstance()->log(trace, "enter the shuffle lower open function.");
	shuffle_ser_obj_->child_->prelude();
	return true;
}

bool ShuffleLower::execute(Block *block) {
	/* get the block from the pipeline. */
	Logging::getInstance()->log(trace, "enter the shuffle lower next function.");
	shuffle_ser_obj_->child_->execute(0);
	return true;
}

bool ShuffleLower::postlude() {
	Logging::getInstance()->log(trace, "enter the shuffle lower close function.");
	shuffle_ser_obj_->child_->postlude();
	return true;
}

}
