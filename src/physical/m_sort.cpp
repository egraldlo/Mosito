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

//bool Sort::prelude() {
//	child_->prelude();
//
//	/* TODO: output_ must be compute. */
//	vector<Expression *> input_=child_->output();
//	schema_=new Schema(&input_);
//
//	vector<SortOrderRef *> sor;
//	for(unsigned i=0;i<expressions_.size();i++) {
//		for(unsigned j=0;j<input_.size();j++) {
//			if(expressions_[i]->expression_->expr_id==input_[j]->expr_id)
//				/* TODO: to do some switch. */
//				sor.push_back(new SortOrderRef(1,expressions_[i]->direction_));
//		}
//	}
//
//	Comparator *compr=new Comparator(schema_, sor);
//
//	lt_=new LoserTree(compr);
//	heap_=new Heap(HEAP_SORT_BUFFER_INIT_SIZE, schema_->get_bytes(), compr);
//	buffer_=new Block(BLOCK_SIZE,schema_->get_bytes());
//
//	void *tuple=0;
//
//	while(child_->execute(buffer_)) {
//		BufferIterator bi=*buffer_->createIterator();
//		while((tuple=bi.getNext())!=0) {
//			if(!heap_->heap_empty()) {
//				if(heap_->init_phase(bi)) {
//					heap_->heap_sort();
//				}
//				else {
//					heap_->heap_sort();
//					/* TODO: return directly. */
//				}
//			}
//			heap_->heap_adjust(tuple);
//		}
//	}
//	heap_->cleanup();
////	getchar();
//
//	/* loser tree buffer to merge the sequential data. */
//	vector<string> files=heap_->get_files();
//	unsigned files_no=files.size();
//	void** filesArray=new void*[files_no+1];
//	int block_size=LOOSE_TREE_BUFFER_NO*schema_->get_bytes();
//	files_=new FILE*[files_no];
//	lt_buffer_=new Block*[files_no];
//	lt_buffer_iterator_=new BufferIterator*[files_no];
//	for(unsigned i=0;i<files_no;i++) {
//		files_[i]=fopen(files[i].c_str(),"rb+");
//		lt_buffer_[i]=new Block(block_size+4, schema_->get_bytes());
//		fread(lt_buffer_[i]->getAddr(),1,block_size,files_[i]);
//		lt_buffer_[i]->assembling(block_size+4,schema_->get_bytes());
//		lt_buffer_iterator_[i]=lt_buffer_[i]->createIterator();
//		filesArray[i+1]=lt_buffer_iterator_[i]->getNext();//todo: some bug in getNext();
//	}
//	lt_->initialize(filesArray,files_no);
//	return true;
//}
//
//bool Sort::execute(Block *block) {
//	cout<<"in the execute of the sort."<<endl;
//	void *tuple=0;
//	void *temp=0;
//	int block_size=LOOSE_TREE_BUFFER_NO*schema_->get_bytes();
//	block->reset();
//	while((tuple=block->allocateTuple())!=0) {
//		int winner=lt_->Winner();
//		block->storeTuple(tuple,lt_->Win());
//		print_tuple(tuple);
//		if((temp=lt_buffer_iterator_[winner-1]->getNext())==0) {
//			/* the data in buffer is used up. */
//			memset(lt_buffer_[winner-1]->getAddr(),'\0',block_size);
//			int fsize=0;
//			if((fsize=fread(lt_buffer_[winner-1]->getAddr(),1,block_size,files_[winner-1]))
//					!=block_size) {
//				/* we must handle with the last tuple to end, like we can set the last to be max. */
//				/* todo: find a good solution. */
//				already_finish_++;
//				maxLast(lt_buffer_[winner-1]->getAddr()+fsize,schema_);
//				if(already_finish_==heap_->get_files().size())
//					return false;
//			}
//			lt_buffer_[winner-1]->reset();
//			lt_buffer_iterator_[winner-1]->reset();
//			temp=lt_buffer_iterator_[winner-1]->getNext();
//		}
//		lt_->Load(winner,temp);
//		if(lt_->replay(winner)!=0)
//			break;
//	}
//	getchar();
//	cout<<"one block is full and can be sent out."<<endl;
//	return true;
//}

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
	child_->prelude();
	ns_=child_->newoutput();

	Logging::getInstance()->log(trace, "will get all the data and sort.");
	void *tuple=0;
	buffer_=new Block(BLOCK_SIZE, ns_->get_bytes());
	schema_=new Schema(ns_);
	while(child_->execute(buffer_)) {
		Block *block=new Block(BLOCK_SIZE, ns_->get_bytes());
		block->storeBlock(buffer_->getAddr(), BLOCK_SIZE);
		blocks_.push_back(block);
		BufferIterator *bi=block->createIterator();
		while((tuple=bi->getNext())!=0) {
			temp_cur_++;
		}
	}

	unsigned every=temp_cur_/CPU_CORE+1;
	unsigned last=temp_cur_-(every)*(CPU_CORE-1);
	for(int i=0; i<CPU_CORE; i++) {
		range rg;
		ranges_.push_back(rg);
	}
	unsigned count=0;
	unsigned r=0;
	for(int i=0; i<blocks_.size(); i++) {
		BufferIterator *bi=blocks_[i]->createIterator();
		while((tuple=bi->getNext())!=0) {
			if(r!=CPU_CORE-1) {
				if(count++<every) {
					ranges_[r].push_back(tuple);
				}
				else {
					count=1;
					r++;
					ranges_[r].push_back(tuple);
				}
			}
			else {
				if(count++<last) {
					ranges_[r].push_back(tuple);
				}
			}
		}
	}

	for(int i=0; i<CPU_CORE; i++) {
		pthread_create(&pths_[i], 0, single_sort, &ranges_[i]);
	}
	for(int i=0; i<CPU_CORE; i++) {
		pthread_join(pths_[i], 0);
	}

	Logging::getInstance()->log(trace, "finished sorting by using multiple threads.");

	return true;
}

bool Sort::execute(Block *block) {
	/*
	 * if we merge the sorted array, we spend 16s.
	 * if we dont merge, we spend 9s, so merge spend much.
	 *  */
	while(temp_cur_){
		void *desc=0;
		block->reset();
		while((desc=block->allocateTuple())){
			void *tuple=heap_out();
			block->storeTuple(desc, tuple);
			if(temp_cur_--)
				continue;
			else
				break;
		}
		block->assembling(BLOCK_SIZE, ns_->get_bytes());
		return true;
	}
	return false;
}

bool Sort::postlude() {
	child_->postlude();
	return true;
}

void *Sort::single_sort(void *args) {
	range *pargs=(range *)args;
	stable_sort(pargs->begin(), pargs->end(), compare);
}

void *Sort::heap_out() {
	void *most=*(ranges_[0].end()-1);
	int most_cur=0;
	for(int i=1; i<CPU_CORE; i++) {
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

bool Sort::compare(const void *left, const void *right) {
	if(*(int *)((char *)left+8)>*(int *)((char *)right+8))
		return false;
	else
		return true;
}

NewSchema *Sort::newoutput(){
	return ns_;
};

}
