/*
 * m_tree_node.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TREE_NODE_H_
#define M_TREE_NODE_H_

#include "m_buffer.h"

#include <vector>
using namespace std;

class TreeNode {
public:
	TreeNode(){};
	virtual ~TreeNode(){};
};

class BinaryNode: public TreeNode {
public:
	BinaryNode(){};
	virtual ~BinaryNode(){};

public:
	TreeNode *left;
	TreeNode *right;

};

class LeafNode: public TreeNode {
public:
	LeafNode(){};
	virtual ~LeafNode(){};

};

class UnaryNode: public TreeNode {
public:
	UnaryNode(){};
	virtual ~UnaryNode(){};

public:
	TreeNode *child;

};

#endif /* M_TREE_NODE_H_ */
