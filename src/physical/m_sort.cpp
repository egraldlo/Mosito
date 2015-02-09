/*
 * m_sort.cpp
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#include "m_sort.h"

namespace physical {

Sort::Sort(vector<SortOrder *> expressions, QueryPlan *child, bool global)
:expressions_(expressions),child_(child),global_(global) {

}

Sort::Sort(QueryPlan *qp)
:child_(qp) {

}

Sort::~Sort() {

}

vector<Expression *> Sort::output() {
	return child_->output();
}

bool Sort::maxLast(void *start, Schema *schema) {
	char *p=(char *)malloc(schema->get_bytes());
	for(int i=0;i<schema->get_columns();i++) {
		if(schema->getDataType(i)->get_type()==t_long) {
			*(unsigned long *)(schema->get_addr(p,i))=ULONG_MAX;
		}
		if(schema->getDataType(i)->get_type()==t_int) {
			*(int *)(schema->get_addr(p,i))=INT_MAX;
		}
	}
	start=p;
}

bool Sort::prelude() {
	sort_ser_obj_->child_->prelude();
//	ns_=child_->newoutput();

	Logging::getInstance()->log(trace, "will get all the data and sort.");
	void *tuple=0;
	temp_cur_=0;
	buffer_=new Block(BLOCK_SIZE, (sort_ser_obj_->ns_).get_bytes());
	schema_=new Schema(&(sort_ser_obj_->ns_));
	while(sort_ser_obj_->child_->execute(buffer_)) {
		Block *block=new Block(BLOCK_SIZE, schema_->get_bytes());
		block->storeBlock(buffer_->getAddr(), BLOCK_SIZE);
		blocks_.push_back(block);
		BufferIterator *bi=block->createIterator();
		while((tuple=bi->getNext())!=0) {
			temp_cur_++;
		}
	}
//	 dist_ranges_.push_back(625000);
//	dist_ranges_.push_back(12500000);
//	dist_ranges_.push_back(1875000);
//	dist_ranges_.push_back(2500000);
//	dist_ranges_.push_back(3125000);
//	dist_ranges_.push_back(37500000);
//	dist_ranges_.push_back(4375000);
	dist_ranges_.push_back(2500);
	dist_ranges_.push_back(5000);
//	dist_ranges_.push_back(5625000);
//	dist_ranges_.push_back(62500000);
//	dist_ranges_.push_back(6875000);
//	dist_ranges_.push_back(7500000);
//	dist_ranges_.push_back(8125000);
//	dist_ranges_.push_back(87500000);
//	dist_ranges_.push_back(9375000);
	dist_ranges_.push_back(7500);
	dist_ranges_.push_back(10000);

	for(int i=0; i<CPU_CORE; i++) {
#ifndef MULTI_PARTITION
		range rg;
#endif
#ifdef MULTI_PARTITION
		range rg;
//		range *rg=new range();
//		pthread_mutex_init(&rg.lock_, 0);
		SpineLock *sl=new SpineLock();
		rg.lock_=sl;
		vector<void *> ranges;
		rg.ranges=ranges;
		ranges_.push_back(rg);
#endif
#ifndef MULTI_PARTITION
		ranges_.push_back(rg);
#endif
	}

	unsigned r=0;
	/* blocks_ is the vector of the data in memory. */
	startTimer(&time_);

#ifdef MULTI_PARTITION
	/* partition phase. */
	for(int i=0; i<CPU_CORE; i++) {
		Argument argument;
		argument.pthis=this;
		argument.range=i;
		pthread_create(&pths_[i], 0, single_partition, &argument);
	}
	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
	}
	cout<<"the partition time consume: "<<getSecond(time_)<<" total "<<endl;

	/* sort phase. */
	for(int i=0; i<CPU_CORE; i++) {
		vector<void *> *ranges=&ranges_[i].ranges;
		cout<<"range number: "<<ranges->size()<<endl;
		pthread_create(&pths_[i], 0, single_sort, ranges);
	}
	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
	}
	count_=0;
#endif

#ifndef MULTI_PARTITION
	for(int i=0; i<blocks_.size(); i++) {
		BufferIterator *bi=blocks_[i]->createIterator();
		while((tuple=bi->getNext())!=0) {
			unsigned long value=*(unsigned long *)((char *)tuple+8);
			r=compare_start_end(value);
			ranges_[r].push_back(tuple);
		}
	}
	cout<<"the partition time consume: "<<getSecond(time_)<<" total "<<endl;

	/* sort phase. */
	for(int i=0; i<CPU_CORE; i++) {
		pthread_create(&pths_[i], 0, single_sort, &ranges_[i]);
	}
	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
		cout<<"ranges: "<<ranges_[i].size()<<endl;
	}
	count_=0;
#endif

	Logging::getInstance()->log(error, "finished sorting by using multiple threads.");
	cout<<"the sort time consume: "<<getSecond(time_)<<" total "<<endl;

	return true;
}

bool Sort::execute(Block *block) {
    /*
     * sequentially read from the ranges_[0]--[CPU_CORE-1]
     * */
	while(temp_cur_){
		void *desc=0;
		void *tuple=0;
		block->reset();
		while((desc=block->allocateTuple())){
#ifdef MULTI_PARTITION
			if(!ranges_[count_].ranges.empty()) {
				tuple=*(ranges_[count_].ranges.end()-1);
				block->storeTuple(desc, tuple);
				ranges_[count_].ranges.pop_back();
				--temp_cur_;
				if(ranges_[count_].ranges.empty()) {
					count_++;
				}
			}
			else {
				count_++;
			}
#endif
#ifndef MULTI_PARTITION
			if(!ranges_[count_].empty()) {
				tuple=*(ranges_[count_].end()-1);
				block->storeTuple(desc, tuple);
				ranges_[count_].pop_back();
				--temp_cur_;
				if(ranges_[count_].empty()) {
					cout<<""<<endl;
					count_++;
				}
			}
			else {
				count_++;
			}
#endif
			if(temp_cur_==0) {
				break;
			}
		}
		block->assembling(BLOCK_SIZE, (sort_ser_obj_->ns_).get_bytes());
		return true;
	}
	return false;
}

bool Sort::postlude() {
//	blocks_.clear();
//	ranges_.clear();
//	buffer_->~Buffer();
	sort_ser_obj_->child_->postlude();
//	cout<<"数组的个数为： "<<count_<<endl;
	return true;
}

#ifdef MULTI_PARTITION
void *Sort::single_partition(void *args) {
	Argument *argument=(Argument *)args;
	BufferIterator *bi=0;
	void *tuple=0;
	for(unsigned i=argument->range; i<argument->pthis->blocks_.size(); ) {
		bi=argument->pthis->blocks_[i]->createIterator();
		unsigned long value;
		int part=0;
		while((tuple=bi->getNext())!=0) {
			value=*(unsigned long *)((char *)tuple+8);
			part=value/2500;
			argument->pthis->ranges_[part].put(tuple);
		}
		i=i+CPU_CORE;
	}
	return 0;
}
#endif

void *Sort::single_sort(void *args) {
#ifdef MULTI_PARTITION
	vector<void *> *pargs=(vector<void *> *)args;
#endif
#ifndef MULTI_PARTITION
	range *pargs=(range *)args;
#endif
	stable_sort(pargs->begin(), pargs->end(), compare);
}
#ifndef MULTI_PARTITION
void *Sort::heap_out() {
	/* ugly and slow way, must be changed to loser tree or heap. */
	void *most=0;
	int most_cur=0;
	for(int i=0; i<CPU_CORE; i++) {
		if(!ranges_[i].empty()) {
			most=*(ranges_[i].end()-1);
			most_cur=i;
			break;
		}
	}
	for(int i=most_cur+1; i<CPU_CORE; i++) {
		if(!ranges_[i].empty()) {
			if(compare(most, *(ranges_[i].end()-1))) {
				most_cur=i;
				most=*(ranges_[i].end()-1);
			}
		}
	}
	ranges_[most_cur].pop_back();
	return most;
}
#endif

bool Sort::compare(const void *left, const void *right) {
	if(*(unsigned long *)((char *)left+8)<*(unsigned long *)((char *)right+8))
		return false;
	else
		return true;
}

NewSchema *Sort::newoutput(){
	return &(sort_ser_obj_->ns_);
};

int Sort::compare_start_end(unsigned long value) {
	for(int i=0; i<dist_ranges_.size(); i++) {
		if(value<dist_ranges_[i]) return i;
	}
}

}
