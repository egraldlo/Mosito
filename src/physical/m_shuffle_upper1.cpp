/*
 * m_shuffle_upper1.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: Casa
 */

#include "m_shuffle_upper1.h"

#include "../../src/executor/m_executor.h"

namespace physical {

/* when test the shuffleupper, this object is been constructed.
 * the first step can be two level, one upper and two lowers.
 * the secod step can be three level, one upper and two mids and three lowers.
 * */
ShuffleUpper1SerObj::ShuffleUpper1SerObj(NewSchema ns, vector<string> uppers, vector<string> lowers,
		QueryPlan *child, int exchange_id)
:ns_(ns), upper_seqs_(uppers), lower_seqs_(lowers), child_(child), exchange_id_(exchange_id){

}

ShuffleUpper1SerObj::~ShuffleUpper1SerObj() {

}

ShuffleUpper1::ShuffleUpper1(vector<Expression *> expressions, QueryPlan *child) {

}

ShuffleUpper1::ShuffleUpper1(Merger * merger) {
	merger_=merger;
}

ShuffleUpper1::~ShuffleUpper1() {

}

bool ShuffleUpper1::prelude() {
	/* send the task into shuffle lower. */
	Logging::getInstance()->log(trace, "enter the shuffle upper open function.");
	/* ugly way. it means that we must select one master to send the tasks. */
	if(Configuration::getInstance()->get_worker_ip()==shuffle_ser_obj_->upper_seqs_[0]) {
		/* todo: modify here, the port_base is for testing. */
		merger_=new Merger(shuffle_ser_obj_->lower_seqs_.size(), PORT_BASE+shuffle_ser_obj_->exchange_id_);
		merger_->m_socket();
		Logging::getInstance()->log(error, "serialize the task and send the task to remote node.");

		cout<<"the upper size is: "<<shuffle_ser_obj_->upper_seqs_.size()<<endl;
		/* before we send the task out, we must check whether other uppers are ready. */
		if(shuffle_ser_obj_->upper_seqs_.size()!=1) {
			if(gather_all()==true) {
				Logging::getInstance()->log(error, "ready for send the task out!");
			}
		}
		/* waiting for accepting the connection. */
		usleep(10000);
		serialization();
	}
	else {
		/*
		 * todo: modify here, the port_base is for testing.
		 * todo: here +1 just for "2" of 1-2-3.
		 * */
		merger_=new Merger(shuffle_ser_obj_->lower_seqs_.size(), PORT_BASE+shuffle_ser_obj_->exchange_id_);
		merger_->m_socket();
		if(send_gather()==true) Logging::getInstance()->log(error, "receive the signal!");
	}
	merger_->m_accept();


	pcbuffer_=new PCBuffer(shuffle_ser_obj_->ns_, shuffle_ser_obj_->lower_seqs_.size());
	block_temp_=new Block(BLOCK_SIZE, shuffle_ser_obj_->ns_.get_bytes());
	meet_zero_=0;
	debug_count_=0;

	/* pthread a receive thread and gather the blocks in the pcbuffer. */
	if(pthread_create(&receive_p_, 0, receive_route, this)==0) {
		Logging::getInstance()->log(trace, "create a receive thread to gather the data.");
	}
	else {
		Logging::getInstance()->log(error, "error in create a receive thread.");
	}

	return true;
}

bool ShuffleUpper1::execute(Block *block) {
	/* it's a consumer, if the buffer has blocks and pipeline it the upper operator. */
	Logging::getInstance()->log(trace, "enter the shuffle upper next function.");
	/* todo: a traverse strategy must be used here. */
	bool empty_or_not_;
	while(1) {
		for(int i=0; i<shuffle_ser_obj_->lower_seqs_.size(); i++) {
			/* todo: a ugly coding here, must use a general way. */
			empty_or_not_=pcbuffer_->get(block_temp_, i);
			if(empty_or_not_==true) {
				block->reset();
				block->storeBlock(block_temp_->getAddr(), BLOCK_SIZE);
				if(block->get_size()==0) {
					if(++meet_zero_==shuffle_ser_obj_->lower_seqs_.size()) {
						pthread_join(receive_p_,0);
						return false;
					}
				}
				Logging::getInstance()->log(trace, "get a block from the buffer and pipeline it.");
				return true;
			}
			else {
				continue;
			}
		}
	}
	/* todo: no return false in this function. */
	return true;
}

bool ShuffleUpper1::postlude() {
//	shuffle_ser_obj_->child_->postlude();
//	merger_->m_close();
	Logging::getInstance()->log(error, "enter the shuffle upper close function.");
	return true;
}

bool ShuffleUpper1::serialization() {
	ShuffleLowerSerObj *slso=new
			ShuffleLowerSerObj(shuffle_ser_obj_->ns_, shuffle_ser_obj_->upper_seqs_,
					shuffle_ser_obj_->child_, shuffle_ser_obj_->exchange_id_);
	ShuffleLower *sl=new ShuffleLower(slso);
	/*
	 * send the serialized tasks to the lower nodes.
	 * here, we need the actor mode of master node and slave nodes.
	 * */
	TaskInfo tasks(sl);
	Message1 serialized_task=TaskInfo::serialize(tasks);
	Logging::getInstance()->log(trace, "ready for send the task to multiple nodes.");

	for(int i=0; i<shuffle_ser_obj_->lower_seqs_.size(); i++) {
		ExecutorMaster::getInstance()->sendToMultiple(serialized_task, shuffle_ser_obj_->lower_seqs_[i]);
	}
	return true;
}

bool ShuffleUpper1::gather_all() {
	int receive_=0;
	cout<<"hello=========================================="<<endl;
	Merger *m=new Merger(shuffle_ser_obj_->upper_seqs_.size()-1, 10000+shuffle_ser_obj_->exchange_id_);
	Sender *s=new Sender(10001+shuffle_ser_obj_->exchange_id_);
	m->m_socket();
	if(m->m_accept()==true) {
		for(int i=0; i<shuffle_ser_obj_->upper_seqs_.size()-1; i++) {
			s->m_connect(shuffle_ser_obj_->upper_seqs_[i+1]);
		}
		s->m_close();
		m->m_close();
		return true;
	};
}

bool ShuffleUpper1::send_gather() {
	cout<<"hello8888888888888888888888888888888888888888888888"<<endl;
	Merger *m=new Merger(1, 10001+shuffle_ser_obj_->exchange_id_);
	Sender *s=new Sender(10000+shuffle_ser_obj_->exchange_id_);
	m->m_socket();
	while(true) {
		if(s->m_connect(shuffle_ser_obj_->upper_seqs_[0])==false) {
			usleep(1000);
			continue;
		}
		else {
			break;
		}
	}
//	while(!s->m_connect(shuffle_ser_obj_->upper_seqs_[0])) {
//		cout<<"waiting....."<<endl;
//	};
	if(m->m_accept()==true) {
		s->m_close();
		m->m_close();
		return true;
	}
}

NewSchema *ShuffleUpper1::newoutput() {
	return &(shuffle_ser_obj_->ns_);
}

void *ShuffleUpper1::receive_route(void *args) {
	ShuffleUpper1 *pthis=(ShuffleUpper1 *)(args);
	if(pthis->merger_->m_receive_select(pthis->pcbuffer_)) {
		Logging::getInstance()->log(trace, "receive all the data the senders will send.");
	}
	return 0;
}

}
