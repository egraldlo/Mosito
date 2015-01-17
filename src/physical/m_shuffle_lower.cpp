/*
 * m_shuffle_lower.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_lower.h"

namespace physical {

ShuffleLowerSerObj::ShuffleLowerSerObj(NewSchema ns, vector<int> seqs, QueryPlan* child, int exchange_id)
:ns_(ns), seqs_(seqs), child_(child), exchange_id_(exchange_id){

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
	shuffle_ser_obj_->child_->prelude();
	/* create a thread to send the block into the upper. */
	Logging::getInstance()->log(trace, "enter the shuffle lower open function.");
	senders_=new Sender*[shuffle_ser_obj_->seqs_.size()];

	/* todo: modify here, the port_base is for testing. */
	for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
		senders_[i]=new Sender(PORT_BASE+shuffle_ser_obj_->exchange_id_);
		senders_[i]->m_connect("127.0.0.1");
	}
	buffer_=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	return true;
}

bool ShuffleLower::execute(Block *block) {
	/* get the block from the pipeline. */
	Logging::getInstance()->log(trace, "enter the shuffle lower next function.");
	Logging::getInstance()->log(trace, "send blocks one by one to the upper nodes.");
	/* senders will send the blocks to the upper nodes. */
	for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
		while(shuffle_ser_obj_->child_->execute(buffer_)){
			senders_[i]->m_send((const char *)buffer_->getAddr(), BLOCK_SIZE);
		}
	}
	return true;
}

bool ShuffleLower::postlude() {
	shuffle_ser_obj_->child_->postlude();
	Logging::getInstance()->log(trace, "enter the shuffle lower close function.");
	return true;
}

}
