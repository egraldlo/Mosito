/*
 * m_bplus_tree.h
 *
 *  Created on: Apr 11, 2015
 *      Author: casa
 */

#ifndef M_BPLUS_TREE_H_
#define M_BPLUS_TREE_H_

#define BPLUSENUM 3
#define VALUE_MAX BPLUSENUM
#define VALUE_MIN (BPLUSENUM/2)
#define CHILD_MAX VALUE_MAX
#define CHILD_MIN (VALUE_MIN+1) //when n+1 points, at least, ceil((n+1)/2) will be used

struct BplusNode {
	int capacity; //in this node, how many values exists
	bool isleaf; //whether a leaf node in this tree
	int keys[BPLUSENUM]; //a points array which can lead to the tuple
	BplusNode* points[BPLUSENUM+1]; //the point to every B tree Node
	BplusNode* next;//this point is used for print tree
};

class BplusTree {
public:
	BplusTree();
	virtual ~BplusTree();

	/* b tree operations */
public:
	bool b_insert(int);
	bool b_delete(int);
	bool b_update(int);
	bool b_select(int);

	void print_tree();

private:
	BplusNode *find_leaf_node(int);

	bool split_node(BplusNode *);
	bool merge_node(BplusNode *);

private:
	BplusNode *root_node;
	/* this node chain is used for print the tree. */
	BplusNode *leaves_node;

};

#endif /* M_BPLUS_TREE_H_ */
