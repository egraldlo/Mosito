/*
 * m_trees.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_trees.h"

Comparator::Comparator(Schema *schema, vector<SortOrderRef *> vs)
:schema_(schema),vs_(vs){

}

Comparator::~Comparator() {

}

bool Comparator::compare(void *left, void *right) {
	int flag;

	for(unsigned i=0;i<vs_.size();i++) {
		void *l=schema_->get_addr(left,vs_[i]->offset_);
		void *r=schema_->get_addr(right,vs_[i]->offset_);

		if(schema_->getDataType(i)->eq(l,r)) {
			flag=0;
		}
		else if((schema_->getDataType(i)->ls(l,r))&&(vs_[i]->direction_==ascending)) {
			flag=1;
		}
		else if((schema_->getDataType(i)->ls(l,r))&&(vs_[i]->direction_==descending)) {
			flag=2;
		}
		else if((schema_->getDataType(i)->gt(l,r))&&(vs_[i]->direction_==descending)) {
			flag=1;
		}
		else if((schema_->getDataType(i)->gt(l,r))&&(vs_[i]->direction_==ascending)) {
			flag=2;
		}
		else {
			/* TODO: a log system must be used here. */
			cout<<"compare error!"<<endl;
		}

		if(flag==0)
			continue;
		if(flag==1){
			return true;
		}
		if(flag==2){
			return false;
		}
	}
	return true;
}

LoserTree::LoserTree() {

}

LoserTree::LoserTree(Comparator *comparator)
:comparator_(comparator){

}

LoserTree::~LoserTree() {
}

void LoserTree::initialize(void* A[], int size) {
	/* initialize some arguments */
	n=size;
	L=A;
	int i,s;

	for(s=1;2*s<=n-1;s+=s);

	/* complete binary tree, evaluate the lowest level nodes' number,
	 * first, we can evaluate the last but one level nodes' number.
	 * */
	lowExt=2*(n-s);
	offset=2*s-1;

	B=new int[size];

	cout<<"     n: "<<n<<endl;
	cout<<"lowExt: "<<lowExt<<endl;
	cout<<"offset: "<<offset<<endl;
	cout<<"in the initialize."<<endl;

	for(i=2;i<=lowExt;i+=2) {
		play((i+offset)/2,i-1,i);
	}

	/* if n is odd number, a parent node and leaf node must be compared. */
	if(n%2) {
		play(n/2,B[(n-1)/2],lowExt+1);
		i=lowExt+3;
	}
	else
		i=lowExt+2;

	for(;i<=n;i+=2) {
		play((i-lowExt+n-1)/2,i-1,i);
	}

//	for(int i=0;i<6;i++){
//		cout<<"B["<<i<<"]: "<<B[i]<<endl;
//	}
	cout<<B[0]<<endl;
}

/*     p: parent node
 *  left: left child node
 * right: right child node
 *  */
void LoserTree::play(int p, int left, int right) {
	/* loser node will be put in the parent position */
	B[p]=lose(L,left,right);
	int temp1,temp2;
	temp1=win(L,left,right);

	/* if parent is in the right child location, it must be recursively play with
	 * the parent node, until it reach the forefathers.
	 *  */
	while(p>1 && p%2) {
		temp2=win(L,temp1,B[p/2]);
		B[p/2]=lose(L,temp1,B[p/2]);
		temp1=temp2;
		p/=2;
	}
	B[p/2]=temp1;
}

int LoserTree::replay(int i) {
	/* find the parent node. */
	int p;
	if(i<=lowExt)
		p=(i+offset)/2;
	else
		p=(i-lowExt+n-1)/2;

	B[0]=win(L,i,B[p]);
	B[p]=lose(L,i,B[p]);

	/* recursively compared with parents. */
	for(;(p/2)>=1;p/=2) {
		int temp;
		temp=win(L,B[p/2],B[0]);
		B[p/2]=lose(L,B[p/2],B[0]);
		B[0]=temp;
	}

	if(L[B[0]]==0)//todo: change L
		return 1;
	else
		return 0;
}

Heap::Heap(int size, int tuple_size) {
	array_=new void*[HEAP_SIZE];
	/*
	 * the following buffer_ can be increase by 2 times.
	 * the function we use here is realloc(), but we can use memory pool.
	 *  */
	flex_buffer_=new PersistFlexBlock(size, tuple_size);
	init_flag_=false;
}

Heap::Heap(int size, int tuple_size, Comparator *comparator) {
	array_=new void*[HEAP_SIZE];
	/*
	 * the following buffer_ can be increase by 2 times.
	 * the function we use here is realloc(), but we can use memory pool.
	 *  */
	flex_buffer_=new PersistFlexBlock(size, tuple_size);
	init_flag_=false;

	comparator_=comparator;
}

Heap::~Heap() {

}

bool Heap::init_phase(BufferIterator &bi) {
	/*
	 * the tuple count is not larger than HEAP_SIZE
	 * return false, else return true;
	 *  */
	init_flag_=true;
	if(bi.get_size()<=HEAP_SIZE) {
		void *tuple=0;
		while((tuple=bi.getNext())!=0) {
			array_[waterline_++]=tuple;
		}
		return false;
	}
	void *tuple=0;
	while((tuple=bi.getNext())!=0) {
		if(waterline_<HEAP_SIZE)
			array_[waterline_++]=tuple;
		else
			break;
	}
	return true;
}

void Heap::heap_sort() {
	for(int i=(waterline_-2)/2;i>=0;i--) {
		heap_again(i,waterline_);
	}
	/*
	 * for test the heap sort.
	 *
	for(int j=0;j<waterline_;j++){
		print_tuple(array_[0]);
		getchar();

		void *temp=array_[0];
		array_[0]=array_[waterline_-j-1];
		array_[waterline_-j-1]=temp;

		heap_again(0,waterline_-j-1);
	}

	fflush(stdin);
	getchar();
	*/
}

void Heap::heap_again(int i, int len) {
	/* left child of the i-th node. */
	int child=2*i+1;
	/* child is the offset, and len is the length, so here child can not
	 * be equaled to len, so child<len is the condition.
	 * */
	while(child<len) {
		/* array[child] < array[child+1], then we use array[child+1] to
		 * compare with the i. if child+1=len, child is the last one.
		 * and only one child. The heap is the minimum heap.
		 * et. "array_[child]<array_[child+1]" means
		 *     comparator_->compare(array_[child], array_[child+1]) is true.
		 *  */
		if(child+1<len && !comparator_->compare(array_[child], array_[child+1]))
			child++;
		/* TODO: how should we go to compare by using the schema and SortOrder vector */
		if(comparator_->compare(array_[i], array_[child]))
			break;
		else {
			void *temp=array_[i];
			array_[i]=array_[child];
			array_[child]=temp;

			i=child;
			/* begin with left child. */
			child=i*2+1;
		}
	}
}

bool Heap::heap_empty() {
	return init_flag_;
}

void Heap::heap_adjust(void *tuple) {
	void *desc=0;
	void *top=heap_get_top();
//	print_tuple(top);
	while((desc=flex_buffer_->allocateTuple())==0) {
		flex_buffer_->double_buffer();
	}
	flex_buffer_->storeTuple(desc,top);

	/* compare the top and tuple */
	if(!comparator_->compare(tuple, top)) {
		array_[0]=tuple;
		heap_again(0, waterline_);
	}
	else {
		waterline_--;
		if(waterline_==0) {
			array_[0]=tuple;
			stringstream fname;
			fname<<"sort_"<<file_off_++;
			files_.push_back(fname.str());
			flex_buffer_->persist(fname.str());
			flex_buffer_->reset();
			waterline_=HEAP_SIZE;
			init_flag_=false;
		}
		else{
			array_[0]=array_[waterline_];
			array_[waterline_]=tuple;
			heap_again(0, waterline_);
		}
//		if(waterline_>1) {
//			array_[0]=array_[waterline_-1];
//			array_[waterline_-1]=tuple;
//			heap_again(0, waterline_-1);
//			waterline_--;
//		}
//		else {
//			while((desc=flex_buffer_->allocateTuple())==0) {
//				flex_buffer_->double_buffer();
//			}
//			flex_buffer_->storeTuple(desc,top);
//			array_[0]=tuple;
//			stringstream fname;
//			fname<<"sort_"<<file_off_++;
//			files_.push_back(fname.str());
//			flex_buffer_->persist(fname.str());
//			flex_buffer_->reset();
//			waterline_=HEAP_SIZE;
//			init_flag_=false;
//		}

//		array_[0]=array_[waterline_-1];
//		array_[waterline_-1]=tuple;
//		waterline_--;
//
//		if(waterline_==0) {
//			stringstream fname;
//			fname<<"sort_"<<file_off_++;
//			files_.push_back(fname.str());
//			flex_buffer_->persist(fname.str());
//			flex_buffer_->reset();
//			waterline_=HEAP_SIZE;
//			init_flag_=false;
//		}
//		else {
//			heap_again(0, waterline_);
//		}

	}
}

bool Heap::cleanup() {
	/* persist the already-ok flex buffer. */
	stringstream fname;
	fname<<"sort_"<<file_off_++;
	files_.push_back(fname.str());
	flex_buffer_->persist(fname.str());

	flex_buffer_->reset();
	waterline_=HEAP_SIZE;

	/* sort and persist the last full heap. */
	heap_sort();
	void *desc=0;
	while(waterline_>=0) {
		heap_again(0,waterline_);
		void *top=heap_get_top();
//		print_tuple(top);
		while((desc=flex_buffer_->allocateTuple())==0) {
			flex_buffer_->double_buffer();
		}
		flex_buffer_->storeTuple(desc,top);
		array_[0]=array_[--waterline_];
	}

	stringstream last;
	last<<"sort_"<<file_off_++;
	files_.push_back(last.str());
	flex_buffer_->persist(last.str());
	return true;
}

void *Heap::heap_get_top() {
	return array_[0];
}

vector<string> Heap::get_files() {
	return files_;
}
