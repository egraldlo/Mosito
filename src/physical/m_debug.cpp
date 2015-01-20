/*
 * m_debug.cpp
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#include "m_debug.h"

namespace physical {

Debug::Debug(vector<Expression *> expressions, QueryPlan *child)
:expressions_(expressions), child_(child) {

}
#ifdef EXPERIMENT
Debug::Debug(QueryPlan *child)
:child_(child){

}
#endif

Debug::~Debug() {

}

bool Debug::prelude() {
	child_->prelude();
	startTimer(&time_);
#ifndef EXPERIMENT_TEST

	/* TODO: output_ must be compute. */
	vector<Expression *> input_=child_->output();
	schema_=new Schema(&input_);
#endif

#ifdef EXPERIMENT_TEST
	NewSchema *input=child_->newoutput();
	schema_=new Schema(input);
#endif

	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
	count_=0;

//	pthread_create(&pt_, 0, timer, &time_);
	return true;
}

bool Debug::execute(Block *) {
	BufferIterator *bi=0;
	void *tuple;
	while(child_->execute(buffer_)) {
		Logging::getInstance()->log(trace,"---------------------");
//		cout<<"actural size: "<<buffer_->getActualSize()<<endl;
		Logging::getInstance()->log(trace,"---------------------");
		bi=buffer_->createIterator();
		while((tuple=bi->getNext())!=0) {
#ifndef TIMING
			display(tuple);
#endif
		}
#ifdef TIMING
	if(++count_ > 8990) {
		cout<<"the query time consume: "<<getSecond(time_)<<endl;
	}
//	cout<<"the query time consume: "<<getSecond(time_)<<endl;
#endif
	}
	return true;
}

bool Debug::postlude() {
	pthread_kill(pt_, SIGTERM);
	Logging::getInstance()->log(trace, "kill the thread!");
	child_->postlude();
	return true;
}

void Debug::display(void *tuple) {
#ifndef EXPERIMENT_TEST
	for(unsigned i=0;i<expressions_.size();i++) {
		expressions_[i]->display(schema_->get_addr(tuple,i));
	}
#endif

#ifdef EXPERIMENT_TEST
	for(unsigned i=0; i<schema_->get_columns(); i++) {
		print(schema_->getDataType(i)->get_type(), schema_->get_addr(tuple, i));
	}
#endif
	cout<<endl;
}

vector<Expression *> Debug::output() {
	return child_->output();
}

void Debug::print(data_type ty,void *attr) {
	/*
	 * TODO: add a spliter ' | '.
	 * */
	if(ty==t_int) {
		cout<<*(int *)attr<<" | ";
	}
	if(ty==t_long) {
		cout<<*(unsigned long*)attr<<" | ";
	}
}

void *Debug::timer(void * args) {
	unsigned long long *t=(unsigned long long *)args;
	while(1) {
		usleep(100000);
		cout<<"the query time consume: "<<getSecond(*t)<<endl;
	}
	return 0;
}

}
