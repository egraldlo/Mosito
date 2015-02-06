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
//		cout<<"the sender socket is: "<<PORT_BASE+shuffle_ser_obj_->exchange_id_<<endl;
//		Logging::getInstance()->log(error, shuffle_ser_obj_->seqs_[i].c_str());
		senders_[i]->m_connect(shuffle_ser_obj_->seqs_[i].c_str());
	}

	buffer_=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	pcbuffer_=new PCBuffer(shuffle_ser_obj_->ns_, shuffle_ser_obj_->seqs_.size());
	meet_zero_=0;
	debug_count_=0;
	count_child_=0;
	ranges_1_.push_back(10000);
//	ranges_2_.push_back(2500000);
	ranges_2_.push_back(2500);
	ranges_2_.push_back(5000);
//	ranges_2_.push_back(7500000);
	ranges_2_.push_back(7500);
	ranges_2_.push_back(10000);

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
	while(true) {
		int range_=0;
		if(shuffle_ser_obj_->child_->execute(buffer_)){
			if(shuffle_ser_obj_->exchange_id_==10)
				range_=buffer_->compare_start_end(ranges_1_);
			else
				range_=buffer_->compare_start_end(ranges_2_);
			if(range_==-1) continue;
			if(shuffle_ser_obj_->exchange_id_!=10)
				pcbuffer_->put(buffer_, range_);
			count_child_++;
		}
		else {
			for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
				buffer_->build(BLOCK_SIZE, 0);
				pcbuffer_->put(buffer_, i);
			}
			cout<<":::::::::"<<count_child_<<endl;
			return false;
		}
	}
	return true;
}

bool ShuffleLower::postlude() {
//	pcbuffer_->~PCBuffer();
	shuffle_ser_obj_->child_->postlude();
	pthread_join(send_p_, 0);
//	delete[] senders_;
	Logging::getInstance()->log(error, "enter the shuffle lower close function.");
	return true;
}

void * ShuffleLower::send_route(void *args) {
	usleep(10000);
	ShuffleLower *pthis=(ShuffleLower *)args;
	Block *block_temp_=new Block(BLOCK_SIZE, pthis->shuffle_ser_obj_->ns_.totalsize_);
	Block *get_block_;
	bool empty_or_not_;
	while(1) {
		for(int i=0; i<pthis->shuffle_ser_obj_->seqs_.size(); i++) {
			/* todo: a ugly coding here, must use a general way. */
			empty_or_not_=pthis->pcbuffer_->get(block_temp_, i);
			if(empty_or_not_==true) {
//				block_temp_->reset();
				stringstream debug_co;
				debug_co<<"-------send already: "<<pthis->debug_count_++;
				Logging::getInstance()->log(trace, debug_co.str().c_str());
//				block_temp_->storeBlock(get_block_->getAddr(), BLOCK_SIZE);
				block_temp_->tag(BLOCK_SIZE, debug_count_);
				pthis->senders_[i]->m_send((const char *)block_temp_->getAddr(), BLOCK_SIZE);
				Logging::getInstance()->log(trace, "send a block to the upper node.");
				if(block_temp_->get_size()==0) {
					if(++pthis->meet_zero_==pthis->shuffle_ser_obj_->seqs_.size()){
//						pthis->senders_[i]->m_close();
						return 0;
					}
				}
			}
			else {
				continue;
			}
		}

//		int range_=0;
//		/* todo: a ugly coding here, must use a general way. */
//		empty_or_not_=pthis->pcbuffer_->get(get_block_, i);
//		if(empty_or_not_==true) {
//			block_temp_->reset();
//			block_temp_->storeBlock(get_block_->getAddr(), BLOCK_SIZE);
//			range_=block_temp_->compare_start_end(ranges_);
//			pthis->senders_[range]->m_send((const char *)block_temp_->getAddr(), BLOCK_SIZE);
//			Logging::getInstance()->log(trace, "send a block to the upper node.");
//			stringstream debug_co;
//			debug_co<<"-------send already: "<<pthis->debug_count_++;
//			Logging::getInstance()->log(trace, debug_co.str().c_str());
//			if(get_block_->get_size()==0) {
//				Logging::getInstance()->log(error, debug_co.str().c_str());
//				return 0;
//			}
//		}
//		else {
//			continue;
//		}
	}
}

}
