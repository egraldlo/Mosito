/*
 * m_trees.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_trees.h"

LoserTree::LoserTree() {

}

LoserTree::~LoserTree() {
}

void LoserTree::initialize(int A[], int size,
	int (*winner)(int A[], int left, int right),
	int (*loser)(int A[], int left, int right)) {
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

	for(i=2;i<=lowExt;i+=2) {
		play((i+offset)/2,i-1,i,winner,loser);
	}

	/* if n is odd number, a parent node and leaf node must be compared. */
	if(n%2) {
		play(n/2,B[(n-1)/2],lowExt+1,winner,loser);
		i=lowExt+3;
	}
	else
		i=lowExt+2;

	for(;i<=n;i+=2) {
		play((i-lowExt+n-1)/2,i-1,i,winner,loser);
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
void LoserTree::play(int p, int left, int right,
	int (*winner)(int A[], int left, int right),
	int (*loser)(int A[], int left, int right)) {
	/* loser node will be put in the parent position */
	B[p]=loser(L,left,right);
	int temp1,temp2;
	temp1=winner(L,left,right);

	/* if parent is in the right child location, it must be recursively play with
	 * the parent node, until it reach the forefathers.
	 *  */
	while(p>1 && p%2) {
		temp2=winner(L,temp1,B[p/2]);
		B[p/2]=loser(L,temp1,B[p/2]);
		temp1=temp2;
		p/=2;
	}
	B[p/2]=temp1;
}

int LoserTree::replay(int i,
	int (*winner)(int A[], int left, int right),
	int (*loser)(int A[], int left, int right)) {
	/* find the parent node. */
	int p;
	if(i<=lowExt)
		p=(i+offset)/2;
	else
		p=(i-lowExt+n-1)/2;

	B[0]=winner(L,i,B[p]);
	B[p]=loser(L,i,B[p]);

	/* recursively compared with parents. */
	for(;(p/2)>=1;p/=2) {
		int temp;
		temp=winner(L,B[p/2],B[0]);
		B[p/2]=loser(L,B[p/2],B[0]);
		B[0]=temp;
	}

	if(L[B[0]]==10000000)
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
	flex_buffer_=new FlexBlock(size, tuple_size);
}

Heap::~Heap() {

}

bool Heap::init_phase(BufferIterator* &bi) {
	/*
	 * the tuple count is not larger than HEAP_SIZE
	 * return false, else return true;
	 *  */
	if(bi->get_size()<=HEAP_SIZE) {
		void *tuple=0;
		while((tuple==bi->getNext())!=0) {
			array_[waterline_++]=tuple;
		}
		return false;
	}
	void *tuple=0;
	while((tuple==bi->getNext())!=0) {
		array_[waterline_++]=tuple;
	}
	return true;
}

void Heap::heap_sort() {
	for(unsigned i=(waterline_-2)/2;i>=0;i--) {
		heap_again(i,waterline_);
	}
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
		 * and only one child.
		 *  */
		if(child+1<len && array_[child]<array_[child+1])
			child++;
		/* TODO: how should we go to compare by using the schema and SortOrder vector */
		if(array_[i]>array_[child])
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

void Heap::heap_adjust(void *tuple) {
	void *desc=0;
	void *top=heap_get_top();
	if((desc=flex_buffer_->allocateTuple())!=0) {
		flex_buffer_->storeTuple(desc,top);
	}
	else {
		flex_buffer_->double_buffer();
		flex_buffer_->storeTuple(desc,top);
	}
	/* compare the top and tuple */

	/*
	 * check whether the heap size is 0,
	 * if 0, rebuild the heap, persist the sorted file.
	 * else, next get top.
	 *  */
}

void *Heap::heap_get_top() {
	return array_[0];
}

void Heap::print(int n) {
	for(int i=0;i<n;i++)
		cout<<"out_["<<i<<"]: "<<out_[i]<<endl;
}
