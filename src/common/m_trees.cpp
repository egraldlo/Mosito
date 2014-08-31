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

Heap::Heap(long size)
:size_(size) {

}

Heap::~Heap() {

}
