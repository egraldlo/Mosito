/*
 * m_shuffle_lower1.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: Casa
 */

#include "m_shuffle_lower1.h"

namespace physical {

ShuffleLower1SerObj::ShuffleLower1SerObj(NewSchema ns, vector<string> seqs, QueryPlan* child, int exchange_id)
:ns_(ns), seqs_(seqs), child_(child), exchange_id_(exchange_id){

}

ShuffleLower1SerObj::~ShuffleLower1SerObj() {

}

ShuffleLower1::ShuffleLower1(vector<Expression *> expressions, QueryPlan *child) {

}

ShuffleLower1::ShuffleLower1(Sender** senders) {
	senders_=senders;
}

ShuffleLower1::~ShuffleLower1() {

}

bool ShuffleLower1::prelude() {
	shuffle_ser_obj_->child_->prelude();
	/* create a thread to send the block into the upper. */
	Logging::getInstance()->log(trace, "enter the shuffle lower open function.");
	senders_=new Sender*[shuffle_ser_obj_->seqs_.size()];
	buffers_=new Block*[CPU_CORE];

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

	ranges_1_.push_back(DATAVOLUME);
	for(int i=0; i<CPU_CORE; i++) {
		ranges_2_.push_back((i+1)*DATAVOLUME/CPU_CORE);
		buffers_[i]=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	}

//	ranges_1_.push_back(50000000);
//	ranges_2_.push_back(12500000);
//	ranges_2_.push_back(25000000);
//	ranges_2_.push_back(37500000);
//	ranges_2_.push_back(50000000);

	/* pthread a send thread to send the blocks out in the pcbuffer. */
	if(pthread_create(&send_p_, 0, send_route, this)==0) {
		Logging::getInstance()->log(trace, "create a send thread to send the data.");
	}
	else {
		Logging::getInstance()->log(error, "error in create a send thread.");
	}

	return true;
}

bool ShuffleLower1::execute(Block *block) {
	/* get the block from the pipeline. */
	Logging::getInstance()->log(trace, "enter the shuffle lower next function.");
	Logging::getInstance()->log(trace, "send blocks one by one to the upper nodes.");
	/*
	 * this function will get the data from the lower pipeline and
	 * store the blocks into pcbuffer.
	 * */
	int mul=CPU_CORE/shuffle_ser_obj_->seqs_.size();
	while(true) {
		int range_=0;
		void *tuple=0;
		void *desc=0;
		if(shuffle_ser_obj_->child_->execute(buffer_)){
			BufferIterator *bi=buffer_->createIterator();
			while((tuple=bi->getNext())!=0) {
				if(shuffle_ser_obj_->exchange_id_==10) {
					range_=partition(tuple, ranges_1_);
				}
				else {
					range_=partition(tuple, ranges_2_);
				}
				if((desc=buffers_[range_]->allocateTuple())!=0) {
					buffers_[range_]->storeTuple(desc, tuple);
				}
				else {
					if(shuffle_ser_obj_->exchange_id_!=10) {
						buffers_[range_]->assembling(BLOCK_SIZE, shuffle_ser_obj_->ns_.totalsize_);
						while(!pcbuffer_->put(buffers_[range_], range_/mul));
					}
					buffers_[range_]->reset();
				}
			}
			count_child_++;
		}
		else {
			for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
				buffer_->build(BLOCK_SIZE, 0);
				while(!pcbuffer_->put(buffer_, i));
			}
			cout<<":::::::::"<<count_child_<<endl;
			return false;
		}
	}
	return true;
}

bool ShuffleLower1::postlude() {
//	pcbuffer_->~PCBuffer();
	shuffle_ser_obj_->child_->postlude();
	pthread_join(send_p_, 0);
//	delete[] senders_;
	Logging::getInstance()->log(error, "enter the shuffle lower close function.");
	return true;
}

void * ShuffleLower1::send_route(void *args) {
	usleep(10000);
	ShuffleLower1 *pthis=(ShuffleLower1 *)args;
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
				block_temp_->tag(BLOCK_SIZE, pthis->debug_count_);
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

int ShuffleLower1::partition(void *tuple, vector<int> bounds) {
	int ret;
	unsigned long start=*(unsigned long *)((char *)tuple+8);
	for(int i=0; i<bounds.size(); i++) {
		if(start<bounds[i]) {
			ret=i;
			break;
		}
	}
	return ret;
}

}

