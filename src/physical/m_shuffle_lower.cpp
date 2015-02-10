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

	/*******************************sort************************************/
	void *tuple=0;
	temp_cur_=0;
	buffers_=new Block*[shuffle_ser_obj_->seqs_.size()];
	buffer0_=new Block(BLOCK_SIZE, (shuffle_ser_obj_->ns_).get_bytes());
	schema_=new Schema(&(shuffle_ser_obj_->ns_));
	while(shuffle_ser_obj_->child_->execute(buffer0_)) {
		Block *block=new Block(BLOCK_SIZE, schema_->get_bytes());
		block->storeBlock(buffer0_->getAddr(), BLOCK_SIZE);
		blocks_.push_back(block);
		BufferIterator *bi=block->createIterator();
		while((tuple=bi->getNext())!=0) {
			temp_cur_++;
		}
	}

	for(int i=0; i<CPU_CORE; i++) {
		vector<void *> rg;
		ranges_.push_back(rg);
	}

	unsigned r=0;
	/* blocks_ is the vector of the data in memory. */
	startTimer(&time_);

	ranges_1_.push_back(50000000);
	ranges_2_.push_back(12500000);
	ranges_2_.push_back(25000000);
	ranges_2_.push_back(37500000);
	ranges_2_.push_back(50000000);

	for(int i=0; i<blocks_.size(); i++) {
		BufferIterator *bi=blocks_[i]->createIterator();
		while((tuple=bi->getNext())!=0) {
			unsigned long value=*(unsigned long *)((char *)tuple+8);
			for(int i=0; i<ranges_2_.size(); i++) {
				if(value<ranges_2_[i]) {
					r=i;
					break;
				}
			}
			ranges_[r].push_back(tuple);
		}
	}
	cout<<"the partition time consume: "<<getSecond(time_)<<" total "<<endl;

	/* sort phase. */
	for(int i=0; i<CPU_CORE; i++) {
		pthread_create(&pths_[i], 0, single_sort1, &ranges_[i]);
	}
	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
		cout<<"ranges: "<<ranges_[i].size()<<endl;
	}
	count_=0;

	cout<<"the sort time consume: "<<getSecond(time_)<<" total "<<endl;

	/*********************************************************************/

	/* create a thread to send the block into the upper. */
	Logging::getInstance()->log(trace, "enter the shuffle lower open function.");
	senders_=new Sender*[shuffle_ser_obj_->seqs_.size()];

//	ranges_1_.push_back(50000000);
//	ranges_2_.push_back(12500000);
//	ranges_2_.push_back(25000000);
//	ranges_2_.push_back(37500000);
//	ranges_2_.push_back(50000000);

	/* todo: modify here, the port_base is for testing. */
	for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
		/* here exchange_id_+i is for m_shuffle_upper.cpp:57 line. */
		buffers_[i]=new Block(BLOCK_SIZE, (shuffle_ser_obj_->ns_).get_bytes());
		senders_[i]=new Sender(PORT_BASE+shuffle_ser_obj_->exchange_id_);
		senders_[i]->m_connect(shuffle_ser_obj_->seqs_[i].c_str());
	}

	buffer_=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	pcbuffer_=new PCBuffer(shuffle_ser_obj_->ns_, shuffle_ser_obj_->seqs_.size());
	one_empty_=false;
	meet_zero_=0;
	debug_count_=0;
	count_child_=0;
	emptys_=0;

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
	vector<bool> oks;
	for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
		oks.push_back(false);
	}
	/*
	while(true) {
		int range_=0;
		if(shuffle_ser_obj_->child_->execute(buffer_)){
			if(shuffle_ser_obj_->exchange_id_==10)
				range_=buffer_->compare_start_end(ranges_1_);
			else
				range_=buffer_->compare_start_end(ranges_2_);
			if(range_==-1) continue;
			if(shuffle_ser_obj_->exchange_id_!=10)
				while(!pcbuffer_->put(buffer_, range_));
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
	return true;*/



	/*
	while(true) {
		void *tuple=0;
		void *desc=0;
		if(ranges_[count_].empty()!=true) {
			buffers_[count_]->reset();
			while((desc=buffers_[count_]->allocateTuple())!=0) {
				tuple=*(ranges_[count_].end()-1);
				buffers_[count_]->storeTuple(desc, tuple);
				ranges_[count_].pop_back();
				--temp_cur_;
				if(ranges_[count_].empty()) {
					cout<<"a range empty."<<endl;
					if(++emptys_==CPU_CORE) {
						for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
							buffers_[i]->build(BLOCK_SIZE, 0);
							while(!pcbuffer_->put(buffers_[i], i));
						}
						cout<<":::::::::"<<count_child_<<endl;
						return false;
					}
					one_empty_=true;
					break;
				}
			}
			count_child_++;
			if(!one_empty_) {
				if(shuffle_ser_obj_->exchange_id_!=10) {
					buffers_[count_]->assembling(BLOCK_SIZE, shuffle_ser_obj_->ns_.totalsize_);
					while(!pcbuffer_->put(buffers_[count_], count_));
				}
			}
			else {
				one_empty_=false;
			}
			count_=(count_+1)%CPU_CORE;
		}
	}*/

	while(true) {
		/* gather */
		void *tuple=0;
		void *desc=0;
		for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
			if(buffers_[i]->empty()&&!ranges_[i].empty()) {
				while((desc=buffers_[i]->allocateTuple())!=0) {
					tuple=*(ranges_[i].end()-1);
					buffers_[i]->storeTuple(desc, tuple);
					ranges_[i].pop_back();
					--temp_cur_;
					if(ranges_[i].empty()) {
						if(++emptys_==CPU_CORE) {
							one_empty_=true;
						}
						break;
					}
				}
			}
		}

		if(one_empty_) {
			while(true) {
				int k=0;
				for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
					if(oks[i]!=true) {
						break;
					}
					if(++k==shuffle_ser_obj_->seqs_.size()) {
						for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
							buffers_[i]->build(BLOCK_SIZE, 0);
							while(!pcbuffer_->put(buffers_[i], i));
						}
						return false;
					}
				}

				for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
					if(!buffers_[i]->empty()) {
						buffers_[i]->assembling(BLOCK_SIZE, shuffle_ser_obj_->ns_.totalsize_);
						if(!pcbuffer_->put(buffers_[i], i)) {
							buffers_[i]->reset();
							oks[i]=true;
						}
					}
					else {
						oks[i]=true;
					}
				}
			}
		}
		else {
			/* send */
			for(int i=0; i<shuffle_ser_obj_->seqs_.size(); i++) {
				if(!buffers_[i]->empty()) {
					buffers_[i]->assembling(BLOCK_SIZE, shuffle_ser_obj_->ns_.totalsize_);
					if(pcbuffer_->put(buffers_[i], i)) {
						cout<<"count: "<<count_child_++<<endl;
						buffers_[i]->reset();
					}
				}
			}
		}
	}
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
	}
}

void *ShuffleLower::single_sort1(void *args) {
	vector<void *> *pargs=(vector<void *> *)args;
	stable_sort(pargs->begin(), pargs->end(), compare1);
}

bool ShuffleLower::compare1(const void *left, const void *right) {
	if(*(unsigned long *)((char *)left+8)<*(unsigned long *)((char *)right+8))
		return false;
	else
		return true;
}

}
