/*
 * m_shuffle_lower.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_lower.h"

namespace physical {

ShuffleLowerSerObj::ShuffleLowerSerObj(NewSchema ns, vector<string> seqs, QueryPlan* child, int exchange_id)
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
		/* here exchange_id_+i is for m_shuffle_upper.cpp:57 line. */
		senders_[i]=new Sender(PORT_BASE+shuffle_ser_obj_->exchange_id_);
		senders_[i]->m_connect(shuffle_ser_obj_->seqs_[i].c_str());
	}

	buffer_=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	pcbuffer_=new PCBuffer(shuffle_ser_obj_->ns_, shuffle_ser_obj_->seqs_.size());
	meet_zero_=false;
	debug_count_=0;

	/* pthread a send thread to send the blocks out in the pcbuffer. */
	if(pthread_create(&send_p_, 0, send_route, this)==0) {
		Logging::getInstance()->log(trace, "create a send thread to send the data.");
	}
	else {
		Logging::getInstance()->log(error, "error in create a send thread.");
	}

	return true;
}

bool ShuffleLower::execute(Block *block) {
	/* get the block from the pipeline. */
	Logging::getInstance()->log(trace, "enter the shuffle lower next function.");
	Logging::getInstance()->log(trace, "send blocks one by one to the upper nodes.");
	/*
	 * this function will get the data from the lower pipeline and
	 * store the blocks into pcbuffer.
	 * */
	while(1) {
		for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
			if(shuffle_ser_obj_->child_->execute(buffer_)){
				pcbuffer_->put(buffer_, i);
		//			senders_[i]->m_send((const char *)buffer_->getAddr(),BLOCK_SIZE);
			}
			else {
				return false;
			}
		}
	}
	return true;
}

bool ShuffleLower::postlude() {
	shuffle_ser_obj_->child_->postlude();
	pthread_join(send_p_, 0);
	Logging::getInstance()->log(error, "enter the shuffle lower close function.");
	return true;
}

void * ShuffleLower::send_route(void *args) {
	ShuffleLower *pthis=(ShuffleLower *)args;
	Block *block_temp_=new Block(BLOCK_SIZE, pthis->shuffle_ser_obj_->ns_.totalsize_);
	Block *get_block_;
	bool empty_or_not_;
	while(1) {
		for(int i=0; i<pthis->shuffle_ser_obj_->seqs_.size(); i++) {
			/* todo: a ugly coding here, must use a general way. */
			empty_or_not_=pthis->pcbuffer_->get(get_block_, i);
			if(empty_or_not_==true) {
				block_temp_->reset();
				block_temp_->storeBlock(get_block_->getAddr(), BLOCK_SIZE);
				pthis->senders_[i]->m_send((const char *)block_temp_->getAddr(), BLOCK_SIZE);
				Logging::getInstance()->log(trace, "send a block to the upper node.");
				stringstream debug_co;
				debug_co<<"-------send already: "<<pthis->debug_count_++;
				Logging::getInstance()->log(trace, debug_co.str().c_str());
				if(get_block_->get_size()==0) {
//					pthis->meet_zero_=true;
					return 0;
				}
			}
			else {
//				if(pthis->meet_zero_==true) return 0;
				continue;
			}
		}
	}
}

}
