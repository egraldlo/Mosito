/*
 * m_srshuffle_upper.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: Casa
 */

#include "m_srshuffle_upper.h"

#include "../../src/executor/m_executor.h"

namespace physical {

/*
 * this shuffle upper don't need to change, because the lower is that range partition function.
 * the upper receive no sequential data and need to followed by a sort operator.
 *  */
SRShuffleUpperSerObj::SRShuffleUpperSerObj(NewSchema ns, vector<string> uppers, vector<string> lowers,
		QueryPlan *child, int exchange_id)
:ns_(ns), upper_seqs_(uppers), lower_seqs_(lowers), child_(child), exchange_id_(exchange_id){

}

SRShuffleUpperSerObj::~SRShuffleUpperSerObj() {

}

SRShuffleUpper::SRShuffleUpper(vector<Expression *> expressions, QueryPlan *child) {

}

SRShuffleUpper::SRShuffleUpper(Merger * merger) {
	merger_=merger;
}

SRShuffleUpper::~SRShuffleUpper() {

}

bool SRShuffleUpper::prelude() {
	/* send the task into shuffle lower. */
	Logging::getInstance()->log(trace, "enter the shuffle upper open function.");
	/* ugly way. it means that we must select one master to send the tasks. */
	if(Configuration::getInstance()->get_worker_ip()==shuffle_ser_obj_->upper_seqs_[0]) {
		/* todo: modify here, the port_base is for testing. */
		merger_=new Merger(shuffle_ser_obj_->lower_seqs_.size(), PORT_BASE+shuffle_ser_obj_->exchange_id_);
		merger_->m_socket();
		Logging::getInstance()->log(error, "serialize the task and send the task to remote node.");

		serialization();
	}
	else {
		/*
		 * todo: modify here, the port_base is for testing.
		 * todo: here +1 just for "2" of 1-2-3.
		 * */
		merger_=new Merger(shuffle_ser_obj_->lower_seqs_.size(), PORT_BASE+shuffle_ser_obj_->exchange_id_);
		merger_->m_socket();
	}
	merger_->m_accept();

	/* waiting for accepting the connection. */
	usleep(10000);

	pcbuffer_=new PCBuffer(shuffle_ser_obj_->ns_, shuffle_ser_obj_->lower_seqs_.size());
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

bool SRShuffleUpper::execute(Block *block) {
	/* it's a consumer, if the buffer has blocks and pipeline it the upper operator. */
	Logging::getInstance()->log(trace, "enter the shuffle upper next function.");

	/* we will use one to multiple model.
	char *data=(char *)malloc(BLOCK_SIZE);
	if(merger_->m_receive(data)) {
		Logging::getInstance()->log(trace, "get a block from the sender!");
		block->storeBlock(data, BLOCK_SIZE);
//		getchar();
		return true;
	}
	else {
		Logging::getInstance()->log(trace, "receive all the blocks from the sender.");
		return false;
	}
	*/

	/* todo: a traverse strategy must be used here. */
	bool empty_or_not_;
	while(1) {
		for(int i=0; i<shuffle_ser_obj_->lower_seqs_.size(); i++) {
			/* todo: a ugly coding here, must use a general way. */
			empty_or_not_=pcbuffer_->get(block_temp_, i);
			if(empty_or_not_==true) {
//				block=block_temp_;
				block->reset();
				block->storeBlock(block_temp_->getAddr(), BLOCK_SIZE);
				if(block->get_size()==0) {
					if(++meet_zero_==shuffle_ser_obj_->lower_seqs_.size())
						return false;
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

bool SRShuffleUpper::postlude() {
//	shuffle_ser_obj_->child_->postlude();
	Logging::getInstance()->log(trace, "enter the shuffle upper close function.");
	pthread_join(receive_p_,0);
	return true;
}

bool SRShuffleUpper::serialization() {
	for(int i=0; i<shuffle_ser_obj_->lower_seqs_.size(); i++) {
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

		ExecutorMaster::getInstance()->sendToMultiple(serialized_task, shuffle_ser_obj_->lower_seqs_[i]);
	}
	return true;
}

NewSchema *SRShuffleUpper::newoutput() {
	return &(shuffle_ser_obj_->ns_);
}

void *SRShuffleUpper::receive_route(void *args) {
	SRShuffleUpper *pthis=(SRShuffleUpper *)(args);
	if(pthis->merger_->m_receive_select(pthis->pcbuffer_)) {
		Logging::getInstance()->log(trace, "receive all the data the senders will send.");
	}
	return 0;
}

}
