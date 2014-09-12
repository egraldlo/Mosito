/*
 * m_project.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_PROJECT_H_
#define M_PROJECT_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_tree_node.h"
#include "../common/m_configuration.h"

namespace physical {

class Project: public UnaryNode, public Iterator {
public:
	/* this can be written as physialplan child */
	Project(vector<Expression *> expressions, Iterator *child);
	virtual ~Project();

	bool prelude();
	bool execute(Block *);
	bool postlude();

private:
	vector<Expression *> expressions_;
	Iterator *child_;

	Block *buffer_;
	BufferIterator *buffer_iterator_;
};

}

#endif /* M_PROJECT_H_ */
