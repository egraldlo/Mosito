/*
 * m_tree_node.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TREE_NODE_H_
#define M_TREE_NODE_H_

class TreeNode {
public:
	TreeNode(){};
	virtual ~TreeNode(){};

};

class Iterator {
public:
	Iterator(){};
	virtual ~Iterator(){};

	virtual bool prelude()=0;
	virtual bool excute()=0;
	virtual bool postlude()=0;

};

class BinaryNode: public TreeNode {
public:
	BinaryNode(){};
	virtual ~BinaryNode(){};

public:
	TreeNode *left;
	TreeNode *right;

};

class LeftNode: public TreeNode {
public:
	LeftNode(){};
	virtual ~LeftNode(){};

};

class UnaryNode: public TreeNode {
public:
	UnaryNode(){};
	virtual ~UnaryNode(){};

public:
	TreeNode *child;

};

#endif /* M_TREE_NODE_H_ */
