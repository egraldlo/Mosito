/*
 * m_trees_test.cpp
 *
 *  Created on: 2014-8-31
 *      Author: casa
 */

#include "../../src/common/m_trees.h"

#include <stdio.h>

#include <string.h>
using namespace std;

int matrix[7][3]={{},{1,4,6},{2,7,8},{3,9,10},{5,20,31},{4,5,90},{10,20,30}};
const string file[6]={"/home/claims/data/file1",
		"/home/claims/data/file2",
		"/home/claims/data/file3",
		"/home/claims/data/file4",
		"/home/claims/data/file5",
		"/home/claims/data/file6"};

int m_losertrees_test() {
	int out[100];
	int array[7];
	FILE **files;
	files=new FILE*[6];

	for(int i=0;i<6;i++) {
		files[i]=fopen(file[i].c_str(),"rb+");
		fread(&array[i+1],4,1,files[i]);
		cout<<array[i+1]<<endl;
	}

	LoserTree *lt=new LoserTree();
	lt->initialize(array,6,LoserTree::win,LoserTree::lose);

	for(int i=0;i<25;i++) {
		int winner=lt->Winner();
		int a;
		fread(&a,4,1,files[winner-1]);
		lt->Load(winner,a);
		if(lt->replay(winner, LoserTree::win,LoserTree::lose)!=0)
			break;
		out[i]=lt->Win();
	}

	for(int i=0;i<25;i++) {
		cout<<"out[i]: "<<out[i]<<endl;
	}

	return 0;
}

int m_heap_test() {
	int in[10]={2,5,1,41,454,6,9,67,32,11};
	Heap *heap=new Heap(10);
	heap->heapSort(in,10);
	heap->print(10);
	return 0;
}
