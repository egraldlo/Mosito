/*
 * m_join.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_join.h"

namespace physical {

HashJoin::HashJoin() {

}

HashJoin::~HashJoin() {

}

bool HashJoin::prelude() {

}

bool HashJoin::execute(Block *) {

}

bool HashJoin::postlude() {

}

void HashJoin::build() {

}

void HashJoin::probe() {

}
#ifdef SYN_JOIN
MergeJoinSerObj::MergeJoinSerObj(NewSchema ns_l, NewSchema ns_r, NewSchema out, QueryPlan *left, QueryPlan *right)
:left_schema_(ns_l), right_schema_(ns_r),output_schema_(out), left_(left), right_(right) {

}

MergeJoinSerObj::~MergeJoinSerObj() {

}

MergeJoin::MergeJoin(
		  vector<Expression *> leftKeys,
		  vector<Expression *> rightKeys,
		  vector<Expression *> conditions,
		  QueryPlan *left,
		  QueryPlan *right,
		  JoinType join_type)
:left_keys_(leftKeys),right_keys_(rightKeys),conditions_(conditions),
 left_(left),right_(right),join_type_(join_type){

}

MergeJoin::~MergeJoin() {

}

/*
 * we collect all data here, before this we will merge them in the shufflelower.
 *  */
bool MergeJoin::prelude() {
	unsigned long long timer_1;
	startTimer(&timer_1);
	Logging::getInstance()->log(error, "enter the open of the merge join... ...");
	left_schema_=new Schema(&(merge_join_ser_obj_->left_schema_));
	right_schema_=new Schema(&(merge_join_ser_obj_->right_schema_));
	output_schema_=new Schema(&(merge_join_ser_obj_->output_schema_));

	unsigned left_tuple_size_=left_schema_->get_bytes();
	unsigned right_tuple_size_=right_schema_->get_bytes();

	left_block_=new Block(BLOCK_SIZE,left_tuple_size_);
	right_block_=new Block(BLOCK_SIZE,right_tuple_size_);

	/*
	 * here we use the universal solution, and we store all data in memory.
	 * it's not a good solution because it will waste a lot of memory space.
	 *  */
	left_flex_block_=new FlexBlock(INIT_FLEX_BLOCK_SIZE,left_tuple_size_);
	right_flex_block_=new FlexBlock(INIT_FLEX_BLOCK_SIZE,right_tuple_size_);

	tablesize_left=0;
	tablesize_right=0;

	pthread_t pths[2];

	Logging::getInstance()->log(error, "store the left tables... ...");
	pthread_create(&pths[0], 0, gather_left, this);

//	merge_join_ser_obj_->left_->prelude();
//	/* collect the left stream data. */
//	while(merge_join_ser_obj_->left_->execute(left_block_)) {
//		lb_itr_=left_block_->createIterator();
//		while((ltuple=lb_itr_->getNext())!=0) {
//			left_flex_block_->storeTupleOK(ltuple);
//			tablesize_left++;
//		}
//	}
//	merge_join_ser_obj_->left_->postlude();

	Logging::getInstance()->log(error, "store the right tables... ...");
	pthread_create(&pths[1], 0, gather_right, this);
//	merge_join_ser_obj_->right_->prelude();
//	/* collect the right stream data. */
//	while(merge_join_ser_obj_->right_->execute(right_block_)) {
//		rb_itr_=right_block_->createIterator();
//		while((rtuple=rb_itr_->getNext())!=0) {
//			right_flex_block_->storeTupleOK(rtuple);
//			tablesize_right++;
//		}
//	}
//	merge_join_ser_obj_->right_->postlude();

	for(int i=0; i<2; i++) {
		pthread_join(pths[i], 0);
	}

	left_flex_block_->assembling(tablesize_left*left_schema_->get_bytes()+4, left_schema_->get_bytes());
	right_flex_block_->assembling(tablesize_right*right_schema_->get_bytes()+4, right_schema_->get_bytes());
	lfb_itr_=left_flex_block_->createIterator();
	rfb_itr_=right_flex_block_->createIterator();

	cout<<"左右表的大小为--： "<<tablesize_left<<"  "<<tablesize_right<<endl;
	cout<<"table size: "<<tablesize_left*left_schema_->get_bytes()<<"table size: "<<tablesize_right*right_schema_->get_bytes()<<endl;
	cout<<"the merge join time consume: "<<getSecond(timer_1)<<endl;

	over_=false;
	return true;
}

void* MergeJoin::gather_left(void *args) {
	MergeJoin *mj=(MergeJoin *)args;
	mj->merge_join_ser_obj_->left_->prelude();
	/* collect the left stream data. */
	void *ltuple=0;
	while(mj->merge_join_ser_obj_->left_->execute(mj->left_block_)) {
		mj->lb_itr_=mj->left_block_->createIterator();
		while((ltuple=mj->lb_itr_->getNext())!=0) {
			mj->left_flex_block_->storeTupleOK(ltuple);
			mj->tablesize_left++;
		}
	}
	mj->merge_join_ser_obj_->left_->postlude();
	return 0;
}

void* MergeJoin::gather_right(void *args) {
	MergeJoin *mj=(MergeJoin *)args;
	mj->merge_join_ser_obj_->right_->prelude();
	/* collect the right stream data. */
	void *rtuple=0;
	while(mj->merge_join_ser_obj_->right_->execute(mj->right_block_)) {
		mj->rb_itr_=mj->right_block_->createIterator();
		while((rtuple=mj->rb_itr_->getNext())!=0) {
			mj->right_flex_block_->storeTupleOK(rtuple);
			mj->tablesize_right++;
		}
	}
	mj->merge_join_ser_obj_->right_->postlude();
	return 0;
}

void print(data_type ty,void *attr) {
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

bool MergeJoin::execute(Block *block) {
	/* merge the two tables and output block. */
	count_=0;
	void *desc=0;
	void *l=lfb_itr_->getNext();
	void *r=rfb_itr_->getNext();
	void *fake_l=0;
	void *fake_r=0;

	block->reset();

	while(l&&r) {
		if(compare(l, r)==1) {
			if((desc=block->allocateTuple())!=0) {
				combine(desc, l, r);
				block->updateFree();
				++count_;
			}
			else {
				block->build(BLOCK_SIZE, count_);
				return true;
			}
			fake_l=lfb_itr_->getNextFake();
			fake_r=rfb_itr_->getNextFake();
			while(fake_l!=0) {
				if(compare(fake_l, r)==1) {
					if((desc=block->allocateTuple())!=0) {
						combine(desc, fake_l, r);
						block->updateFree();
						++count_;
					}
					else {
						block->build(BLOCK_SIZE, count_);
						return true;
					}
					fake_l=lfb_itr_->getNextFake();
				}
				else {
					break;
				}
			}
			while(fake_r!=0) {
				if(compare(l, fake_r)==1) {
					if((desc=block->allocateTuple())!=0) {
						combine(desc, l, fake_r);
						block->updateFree();
						++count_;
					}
					else {
						block->build(BLOCK_SIZE, count_);
						return true;
					}
					fake_r=lfb_itr_->getNextFake();
				}
				else {
					break;
				}
			}
			l=lfb_itr_->getNext();
			r=rfb_itr_->getNext();
		}
		else if(compare(l, r)==0){
			l=lfb_itr_->getNext();
		}
		else {
			r=rfb_itr_->getNext();
		}
	}
	cout<<"----------------->merge all the tuples."<<endl;
	return false;
}

int MergeJoin::compare(void *left, void *right) {
	/*
	 * by using '=' expression and filter out the equal expression
	 * todo: a binding strategy to bind the 'conditions_' to child's input
	 * left_keys_ and right_keys_,
	 * */
	if(*(unsigned long *)((char *)left+8)==*(unsigned long *)((char *)right+8))
		return 1;
	if(*(unsigned long *)((char *)left+8)<*(unsigned long *)((char *)right+8))
		return 0;
	if(*(unsigned long *)((char *)left+8)>*(unsigned long *)((char *)right+8))
		return -1;
}

bool MergeJoin::combine(void *&des, void *left, void *right) {
	memcpy(des, left, left_schema_->get_bytes());
	memcpy(des+left_schema_->get_bytes(), right, right_schema_->get_bytes());
	return true;
}

bool MergeJoin::postlude() {
	Logging::getInstance()->log(error, "the merge join finished........");
	return true;
}

vector<Expression *> MergeJoin::output() {
	return left_->output();
}

NewSchema *MergeJoin::newoutput() {
	return &(merge_join_ser_obj_->output_schema_);
}
#endif

#ifndef SYN_JOIN
MergeJoinSerObj::MergeJoinSerObj(NewSchema ns_l, NewSchema ns_r, NewSchema out, QueryPlan *left, QueryPlan *right)
:left_schema_(ns_l), right_schema_(ns_r),output_schema_(out), left_(left), right_(right) {

}

MergeJoinSerObj::~MergeJoinSerObj() {

}

MergeJoin::MergeJoin(
		  vector<Expression *> leftKeys,
		  vector<Expression *> rightKeys,
		  vector<Expression *> conditions,
		  QueryPlan *left,
		  QueryPlan *right,
		  JoinType join_type)
:left_keys_(leftKeys),right_keys_(rightKeys),conditions_(conditions),
 left_(left),right_(right),join_type_(join_type){

}

MergeJoin::~MergeJoin() {

}

/*
 * we collect all data here, before this we will merge them in the shufflelower.
 *  */
bool MergeJoin::prelude() {
	unsigned long long timer_1;
	startTimer(&timer_1);
	Logging::getInstance()->log(error, "enter the open of the merge join... ...");
	left_schema_=new Schema(&(merge_join_ser_obj_->left_schema_));
	right_schema_=new Schema(&(merge_join_ser_obj_->right_schema_));
	output_schema_=new Schema(&(merge_join_ser_obj_->output_schema_));
	unsigned left_tuple_size_=left_schema_->get_bytes();
	unsigned right_tuple_size_=right_schema_->get_bytes();
	left_block_=new Block(BLOCK_SIZE,left_tuple_size_);
	right_block_=new Block(BLOCK_SIZE,right_tuple_size_);
	left_flex_block_=new FlexBlock*[CPU_CORE];
	right_flex_block_=new FlexBlock*[CPU_CORE];
	lfb_itr_=new BufferIterator*[CPU_CORE];
	rfb_itr_=new BufferIterator*[CPU_CORE];

	/*
	 * here we use the universal solution, and we store all data in memory.
	 * it's not a good solution because it will waste a lot of memory space.
	 *  */
	for(int i=0; i<CPU_CORE; i++) {
		left_flex_block_[i]=new FlexBlock(INIT_FLEX_BLOCK_SIZE,left_tuple_size_);
		right_flex_block_[i]=new FlexBlock(INIT_FLEX_BLOCK_SIZE,right_tuple_size_);
		tablesize_left[i]=0;
		tablesize_right[i]=0;
	}

//	Sort *s1=(Sort *)(merge_join_ser_obj_->right_);
//	ShuffleUpper1 *su1=(ShuffleUpper1 *)(s1->child);
//	int th=0;
//	string ip=Configuration::getInstance()->get_worker_ip();
//	for(int i=0; i<su1->shuffle_ser_obj_->lower_seqs_.size(); i++) {
//		if(ip==su1->shuffle_ser_obj_->lower_seqs_[i]) {
//			th=i;
//			break;
//		}
//	}

	pthread_t pths[2];
	Logging::getInstance()->log(error, "store the left tables... ...");
	pthread_create(&pths[0], 0, gather_left, this);
	Logging::getInstance()->log(error, "store the right tables... ...");
	pthread_create(&pths[1], 0, gather_right, this);
	for(int i=0; i<2; i++) {
		pthread_join(pths[i], 0);
	}

//	int th=s1->th;
//
//	for(int i=0; i<CPU_CORE; i++) {
//		int value=th*DATAVOLUME/4+(i+1)*DATAVOLUME/(4*CPU_CORE);
//		dist_ranges_.push_back(value);
//		cout<<"dist_ranges_: "<<dist_ranges_[i]<<endl;
//	}

	for(int i=0; i<CPU_CORE; i++) {
		left_flex_block_[i]->assembling(tablesize_left[i]*left_schema_->get_bytes()+4, left_schema_->get_bytes());
		right_flex_block_[i]->assembling(tablesize_right[i]*right_schema_->get_bytes()+4, right_schema_->get_bytes());
		lfb_itr_[i]=left_flex_block_[i]->createIterator();
		rfb_itr_[i]=right_flex_block_[i]->createIterator();
		cout<<"左右表的大小为--： "<<tablesize_left[i]<<"  "<<tablesize_right[i]<<"   "<<count_o_[i]<<endl;
	}

//	cout<<"table size: "<<tablesize_left*left_schema_->get_bytes()<<"table size: "<<tablesize_right*right_schema_->get_bytes()<<endl;
	cout<<"the merge join time consume: "<<getSecond(timer_1)<<endl;

	pthread_t pths_join[CPU_CORE];
	for(int i=0; i<CPU_CORE; i++) {
		JoinArgu joinargu;
		joinargu.i_=i; joinargu.mj_=this;
		pthread_create(&pths_join[i], 0, gather_merge, &joinargu);
	}

	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_join[i], 0);
	}

	over_=false;
	return true;
}

void* MergeJoin::gather_left(void *args) {
	MergeJoin *mj=(MergeJoin *)args;
	mj->merge_join_ser_obj_->left_->prelude();
	/* collect the left stream data. */
	void *ltuple=0;
	while(mj->merge_join_ser_obj_->left_->execute(mj->left_block_)) {
		mj->lb_itr_=mj->left_block_->createIterator();
		while((ltuple=mj->lb_itr_->getNext())!=0) {
			unsigned long value=*(unsigned long *)((char *)ltuple+8);
			int range=(value)/CPU_CORE;
			mj->left_flex_block_[range]->storeTupleOK(ltuple);
			mj->tablesize_left[range]++;
		}
	}
	mj->merge_join_ser_obj_->left_->postlude();
	return 0;
}

void* MergeJoin::gather_right(void *args) {
	MergeJoin *mj=(MergeJoin *)args;
	mj->merge_join_ser_obj_->right_->prelude();
	/* collect the right stream data. */
	void *rtuple=0;
	while(mj->merge_join_ser_obj_->right_->execute(mj->right_block_)) {
		mj->rb_itr_=mj->right_block_->createIterator();
		while((rtuple=mj->rb_itr_->getNext())!=0) {
			unsigned long value=*(unsigned long *)((char *)rtuple+8);
			int range=(value)/CPU_CORE;
			mj->right_flex_block_[range]->storeTupleOK(rtuple);
			mj->tablesize_right[range]++;
		}
	}
	mj->merge_join_ser_obj_->right_->postlude();
	return 0;
}

void *MergeJoin::gather_merge(void *args) {
	JoinArgu *ja=(JoinArgu*)args;
	ja->mj_->count_[ja->i_]=0;
	void *desc=0;

funck:
	void *l=ja->mj_->lfb_itr_[ja->i_]->getNext();
	void *r=ja->mj_->rfb_itr_[ja->i_]->getNext();
	void *fake_l=0;
	void *fake_r=0;

	Block *block=new Block(BLOCK_SIZE, ja->mj_->output_schema_->get_bytes());
	block->reset();

	while(l&&r) {
		if(ja->mj_->compare(l, r)==1) {
			if((desc=block->allocateTuple())!=0) {
				ja->mj_->combine(desc, l, r);
				block->updateFree();
				++ja->mj_->count_[ja->i_];
			}
			else {
				block->build(BLOCK_SIZE, ja->mj_->count_[ja->i_]);
				block->reset();
				ja->mj_->count_o_[ja->i_]++;
				goto funck;
			}
			fake_l=ja->mj_->lfb_itr_[ja->i_]->getNextFake();
			fake_r=ja->mj_->rfb_itr_[ja->i_]->getNextFake();
			while(fake_l!=0) {
				if(ja->mj_->compare(fake_l, r)==1) {
					if((desc=block->allocateTuple())!=0) {
						ja->mj_->combine(desc, fake_l, r);
						block->updateFree();
						++ja->mj_->count_[ja->i_];
					}
					else {
						block->build(BLOCK_SIZE, ja->mj_->count_[ja->i_]);
						block->reset();
						ja->mj_->count_o_[ja->i_]++;
						goto funck;
					}
					fake_l=ja->mj_->lfb_itr_[ja->i_]->getNextFake();
				}
				else {
					break;
				}
			}
			while(fake_r!=0) {
				if(ja->mj_->compare(l, fake_r)==1) {
					if((desc=block->allocateTuple())!=0) {
						ja->mj_->combine(desc, l, fake_r);
						block->updateFree();
						++ja->mj_->count_[ja->i_];
					}
					else {
						block->build(BLOCK_SIZE, ja->mj_->count_[ja->i_]);
						block->reset();
						ja->mj_->count_o_[ja->i_]++;
						goto funck;
					}
					fake_r=ja->mj_->lfb_itr_[ja->i_]->getNextFake();
				}
				else {
					break;
				}
			}
			l=ja->mj_->lfb_itr_[ja->i_]->getNext();
			r=ja->mj_->rfb_itr_[ja->i_]->getNext();
		}
		else if(ja->mj_->compare(l, r)==0){
			l=ja->mj_->lfb_itr_[ja->i_]->getNext();
		}
		else {
			r=ja->mj_->rfb_itr_[ja->i_]->getNext();
		}
	}
	return 0;
}

int MergeJoin::compare_start_end(unsigned long value) {
	for(int i=0; i<dist_ranges_.size(); i++) {
		if(value<dist_ranges_[i]) return i;
	}
}

void print(data_type ty,void *attr) {
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

bool MergeJoin::execute(Block *block) {
	return false;
}

int MergeJoin::compare(void *left, void *right) {
	/*
	 * by using '=' expression and filter out the equal expression
	 * todo: a binding strategy to bind the 'conditions_' to child's input
	 * left_keys_ and right_keys_,
	 * */
	if(*(unsigned long *)((char *)left+8)==*(unsigned long *)((char *)right+8))
		return 1;
	if(*(unsigned long *)((char *)left+8)<*(unsigned long *)((char *)right+8))
		return 0;
	if(*(unsigned long *)((char *)left+8)>*(unsigned long *)((char *)right+8))
		return -1;
}

bool MergeJoin::combine(void *&des, void *left, void *right) {
	memcpy(des, left, left_schema_->get_bytes());
	memcpy(des+left_schema_->get_bytes(), right, right_schema_->get_bytes());
	return true;
}

bool MergeJoin::postlude() {
	Logging::getInstance()->log(error, "the merge join finished........");
	return true;
}

vector<Expression *> MergeJoin::output() {
	return left_->output();
}

NewSchema *MergeJoin::newoutput() {
	return &(merge_join_ser_obj_->output_schema_);
}
#endif

NestLoopJoinSerObj::NestLoopJoinSerObj(NewSchema ns_l, NewSchema ns_r, NewSchema out, QueryPlan *left, QueryPlan *right)
:left_schema_(ns_l), right_schema_(ns_r),output_schema_(out), left_(left), right_(right) {

}

NestLoopJoinSerObj::~NestLoopJoinSerObj() {

}

NestLoopJoin::NestLoopJoin() {

}

NestLoopJoin::~NestLoopJoin() {

}

bool NestLoopJoin::prelude() {
	return true;
}

bool NestLoopJoin::execute(Block *) {
	return true;
}

bool NestLoopJoin::postlude() {
	return true;
}

NewSchema *NestLoopJoin::newoutput() {
	return &(nestloop_ser_obj_->output_schema_) ;
}

} /* namespace physical */
