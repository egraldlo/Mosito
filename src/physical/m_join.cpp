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

	void *ltuple,*rtuple;

	unsigned tablesize_left=0;
	unsigned tablesize_right=0;

	Logging::getInstance()->log(error, "store the left tables... ...");
	merge_join_ser_obj_->left_->prelude();
	/* collect the left stream data. */
	while(merge_join_ser_obj_->left_->execute(left_block_)) {
		lb_itr_=left_block_->createIterator();
		lb_itr_->reset();
		while((ltuple=lb_itr_->getNext())!=0) {
			left_flex_block_->storeTupleOK(ltuple);
			tablesize_left++;
		}
	}

	Logging::getInstance()->log(error, "store the right tables... ...");
	merge_join_ser_obj_->right_->prelude();
	/* collect the right stream data. */
	while(merge_join_ser_obj_->right_->execute(right_block_)) {
		rb_itr_=right_block_->createIterator();
		rb_itr_->reset();
		while((rtuple=rb_itr_->getNext())!=0) {
			right_flex_block_->storeTupleOK(rtuple);
			tablesize_right++;
		}
	}

	left_flex_block_->assembling(tablesize_left*left_schema_->get_bytes()+4, left_schema_->get_bytes());
	right_flex_block_->assembling(tablesize_right*right_schema_->get_bytes()+4, right_schema_->get_bytes());
	lfb_itr_=left_flex_block_->createIterator();
	rfb_itr_=right_flex_block_->createIterator();

	cout<<"左右表的大小为--： "<<tablesize_left<<"  "<<tablesize_right<<endl;
//	getchar();

	over_=false;
	return true;
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

//	if(over_) {
//		cout<<"this block has tupleshahahahhahahhaha: "<<count_<<endl;
//		getchar();
//		return false;
//	}
	while(l&&r) {
		if(compare(l, r)==1) {
			if((desc=block->allocateTuple())!=0) {
				combine(desc, l, r);
				block->updateFree();
				++count_;
//				for(unsigned i=0; i<output_schema_->get_columns(); i++) {
//					print(output_schema_->getDataType(i)->get_type(),
//							output_schema_->get_addr(desc, i));
//				}
//				cout<<endl<<count_++<<endl;
//				usleep(10000);
			}
			else {
//				cout<<"this block has tuples: "<<count_<<endl;
//				getchar();
				block->build(BLOCK_SIZE, count_);
//				if(!(l&&r))
//					over_=true;
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
//						cout<<"this block has tuples: "<<count_<<endl;
//						getchar();
						block->build(BLOCK_SIZE, count_);
//						if(!(l&&r))
//							over_=true;
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
//						cout<<"this block has tuples: "<<count_<<endl;
//						getchar();
						block->build(BLOCK_SIZE, count_);
//						if(!(l&&r))
//							over_=true;
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
			r=rfb_itr_->getNext();
		}
		else {
			l=lfb_itr_->getNext();
		}
	}
	if(over_) {
		return false;
	}
	else {
		block->build(BLOCK_SIZE, 0);
		over_=true;
		return true;
	}
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
	merge_join_ser_obj_->left_->postlude();
	merge_join_ser_obj_->right_->postlude();
	return true;
}

vector<Expression *> MergeJoin::output() {
	return left_->output();
}

NewSchema *MergeJoin::newoutput() {
	return &(merge_join_ser_obj_->output_schema_);
}

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
