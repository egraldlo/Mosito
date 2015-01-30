/*
 * m_shuffle_upper.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_upper.h"

#include "../../src/executor/m_executor.h"

namespace physical {

/* when test the shuffleupper, this object is been constructed.
 * the first step can be two level, one upper and two lowers.
 * the secod step can be three level, one upper and two mids and three lowers.
 * */
ShuffleUpperSerObj::ShuffleUpperSerObj(NewSchema ns, vector<string> uppers, vector<string> lowers,
		QueryPlan *child, int exchange_id)
:ns_(ns), upper_seqs_(uppers), lower_seqs_(lowers), child_(child), exchange_id_(exchange_id){

}

ShuffleUpperSerObj::~ShuffleUpperSerObj() {

}

ShuffleUpper::ShuffleUpper(vector<Expression *> expressions, QueryPlan *child) {

}

ShuffleUpper::ShuffleUpper(Merger * merger) {
	merger_=merger;
}

ShuffleUpper::~ShuffleUpper() {

}

bool ShuffleUpper::prelude() {
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

	/* pthread a receive thread and gather the blocks in the pcbuffer. */
	if(pthread_create(&receive_p_, 0, receive_route, this)==0) {
		Logging::getInstance()->log(trace, "create a receive thread to gather the data.");
	}
	else {
		Logging::getInstance()->log(error, "error in create a receive thread.");
	}

	return true;
}

bool ShuffleUpper::execute(Block *block) {
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
				if(block->get_size()==0) return false;
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

bool ShuffleUpper::postlude() {
//	shuffle_ser_obj_->child_->postlude();
	Logging::getInstance()->log(trace, "enter the shuffle upper close function.");
	pthread_join(receive_p_,0);
	return true;
}

bool ShuffleUpper::serialization() {
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

NewSchema *ShuffleUpper::newoutput() {
	return &(shuffle_ser_obj_->ns_);
}

void *ShuffleUpper::receive_route(void *args) {
	ShuffleUpper *pthis=(ShuffleUpper *)(args);
	if(pthis->merger_->m_receive_select(pthis->pcbuffer_)) {
		Logging::getInstance()->log(trace, "receive all the data the senders will send.");
	}
	return 0;
}

}
