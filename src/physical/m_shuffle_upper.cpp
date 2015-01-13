/*
 * m_shuffle_upper.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_shuffle_upper.h"

namespace physical {

/* when test the shuffleupper, this object is been constructed.
 * the first step can be two level, one upper and two lowers.
 * the secod step can be three level, one upper and two mids and three lowers.
 * */
ShuffleUpperSerObj::ShuffleUpperSerObj(NewSchema ns, vector<int> uppers, vector<int> lowers, QueryPlan *child)
:ns_(ns), upper_seqs_(uppers), lower_seqs_(lowers), child_(child){

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

	/* here we must create pthread to receive the data, it's a producer.*/
	return true;
}

bool ShuffleUpper::execute(Block *block) {
	/* it's a consumer, if the buffer has blocks and pipeline it the upper operator. */

	return true;
}

bool ShuffleUpper::postlude() {
	return true;
}

bool ShuffleUpper::serialization() {
	NewSchema schema=shuffle_ser_obj_->ns_;
	QueryPlan *child=shuffle_ser_obj_->child_;
	vector<int> upper_nodes=shuffle_ser_obj_->upper_seqs_;
	vector<int> lower_nodes=shuffle_ser_obj_->lower_seqs_;
	ShuffleLowerSerObj *slso=new
			ShuffleLowerSerObj(schema, upper_nodes, child);
	TaskInfo tasks(new ShuffleLower(slso));
	Message1 serialized_task=TaskInfo::serialize(tasks);
	/*
	 * send the serialized tasks to the lower nodes.
	 * here, we need the actor mode of master node and slave nodes.
	 * */
	ExecutorMaster::getInstance()->sendToMultiple(serialized_task, lower_nodes);

	return true;
}

}
