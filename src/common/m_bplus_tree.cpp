/*
 * m_bplus_tree.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: casa
 */

#include "m_bplus_tree.h"

BplusTree::BplusTree() {
	// TODO Auto-generated constructor stub
	root_node=new BplusNode(); 
	root_node->capacity=0;
	root_node->isleaf=true;
	root_node->points[0]=0;
}

BplusTree::~BplusTree() {
	// TODO Auto-generated destructor stub
}

bool BplusTree::b_insert(int key) {
	/* first we find where we must put the key in the tree. */
	BplusNode *waited_insert_node = find_leaf_node(key);

	/* check whether the node is full. */
	if(waited_insert_node->capacity == BPLUSENUM) {
		split_node(waited_insert_node);
	}

	int waited_insert_location = 0;

	for(waited_insert_location=0; waited_insert_location <= waited_insert_node->capacity; waited_insert_location++) {
		if(waited_insert_node->keys[waited_insert_location] > key) {
			break;
		}
	}

	for(int i=waited_insert_node->capacity; i >= waited_insert_location; i--) {
		waited_insert_node[i+1] = waited_insert_node[i];
	}

	for(int i=waited_insert_node->capacity+1; i>waited_insert_location; i--) {

	}

	waited_insert_node->keys[waited_insert_location] = key;



	return true;
}

bool BplusTree::b_delete(int) {

	return true;
}

bool BplusTree::b_update(int) {

	return true;
}

bool BplusTree::b_select(int) {

	return true;
}

void BplusTree::print_tree() {
	/* here we use the deep firstly traverse of the B+ tree. */
	BplusNode * temp = 0;
	while (1) {
		for(int i=0; i<temp->capacity+1; i++) {
			
		}
	}	
}

BplusNode *BplusTree::find_leaf_node(int key) {
	BplusNode *ret;
	BplusNode *tmp = root_node;
	/* we search the key from the root to the leaves. */

	if(tmp->capacity == 0) {
		/* this is the first key in this tree. */
		ret = tmp;
	}

	return ret;
}

bool BplusTree::split_node(BplusNode *waited_spilt_node) {

	return true;
}

bool BplusTree::merge_node(BplusNode *waited_merge_node) {

	return true;
}
