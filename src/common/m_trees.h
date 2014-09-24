/*
 * m_trees.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TREES_H_
#define M_TREES_H_

#include "../common/m_configuration.h"

#include <iostream>
using namespace std;

class LoserTree {
public:
	LoserTree();
	virtual ~LoserTree();

	/* here we define the winner function as function pointer.
	 * because this can adjust to template class, this needs
	 * user definition.
	 *  */
	static int win(int A[], int left, int right) {
		if(A[left]<=A[right])
			return left;
		else
			return right;
	}

	static int lose(int A[], int left, int right) {
		if(A[left]>A[right])
			return left;
		else
			return right;
	}

	void initialize(int A[], int size,
			int (*winner)(int A[], int left, int right),
			int (*loser)(int A[], int left, int right));
	void play(int p, int left, int right,
			int (*winner)(int A[], int left, int right),
			int (*loser)(int A[], int left, int right));
	int replay(int i,
			int (*winner)(int A[], int left, int right),
			int (*loser)(int A[], int left, int right));

	int Winner() {
		return B[0];
	}

	void Load(int win, int newOne) {
		L[win]=newOne;
	}

	int Win() {
		return L[B[0]];
	}
private:
	/* n files merged. */
	int n;
	/* nodes number in the lowest level. */
	int lowExt;
	/* nodes number apart from the lowest level. */
	int offset;
	/* loser tree array, B[0] is the winner, it's out of the tree. */
	int *B;
	/* leaves array
	 * TODO: here we can define it as template.
	 *  */
	int *L;
};

class Heap {
public:
	Heap(int size, int tuple_size);
	virtual ~Heap();

	bool init_phase(void *);

	void heap_again();
	void heap_sort();

	void heap_adjust(void *);
	void* heap_get_top();

	void print(int n);

private:
	/* the waterline of the array_. */
	int waterline_;
	/*
	 * here we allocate a memory space to store the pointer.
	 * */
	void **array_;
	int *out_;
	/* the buffer which can store the data by increasing 2 times. */
	FlexBuffer *flex_buffer_;
	vector<string> file_path_list_;
};

#endif /* M_TREES_H_ */
