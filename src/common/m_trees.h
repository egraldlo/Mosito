/*
 * m_trees.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TREES_H_
#define M_TREES_H_

#include "../expressions/m_sort_order.h"
#include "../common/m_configuration.h"
#include "../common/m_schema.h"
#include "m_buffer.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Comparator {
public:
	Comparator(Schema *schema, vector<SortOrderRef *> vs);
	virtual ~Comparator();

	bool compare(void *, void *);

private:
	Schema *schema_;
	vector<SortOrderRef *> vs_;
};

class LoserTree {
public:
	LoserTree();
	LoserTree(Comparator *comparator);
	virtual ~LoserTree();

	/* here we define the winner function as function pointer.
	 * because this can adjust to template class, this needs
	 * user definition.
	 *  */
	int win(void* A[], int left, int right) {
		if(comparator_->compare(A[left],A[right]))
			return left;
		else
			return right;
	}

	int lose(void* A[], int left, int right) {
		if(!comparator_->compare(A[left],A[right]))
			return left;
		else
			return right;
	}

	void initialize(void* A[], int size);
	void play(int p, int left, int right);

	int replay(int i);

	int Winner() {
		return B[0];
	}

	void Load(int win, void* newOne) {
		L[win]=newOne;
	}

	void* Win() {
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
	void **L;

	Comparator *comparator_;
};

class Heap {
public:
	Heap(int size, int tuple_size);
	Heap(int size, int tuple_size, Comparator *comparator);
	virtual ~Heap();

	/* full the heap at first. */
	bool init_phase(BufferIterator &bi);
	/* sort the heap after init_phase. */
	void heap_sort();

	void heap_again(int, int);

	bool heap_empty();

	/* compare the void * and rebuild the heap. */
	void heap_adjust(void *);

	/* get the array_[0] from the heap. */
	void* heap_get_top();

	bool cleanup();

	vector<string> get_files();

private:
	/* the waterline of the array_. */
	int waterline_;
	/*
	 * here we allocate a memory space to store the pointer.
	 * */
	void **array_;
	int *out_;
	/* the buffer which can store the data by increasing 2 times. */
	PersistFlexBlock *flex_buffer_;
	vector<string> file_path_list_;

	Comparator *comparator_;

	/* whether the heap has data. */
	bool init_flag_;

	unsigned file_off_;
	vector<string> files_;
};

#endif /* M_TREES_H_ */
