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
#include "../common/m_schema.h"
#include "m_query_plan.h"

namespace physical {

class Project: public UnaryNode, public QueryPlan {
public:
	/* this can be written as physical plan child */
	Project(vector<Expression *> expressions, QueryPlan *child);
	virtual ~Project();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output();

private:
	vector<Expression *> expressions_;
	QueryPlan *child_;

	Block *buffer_;
	BufferIterator *buffer_iterator_;

	Schema *schema_;
};

}

#endif /* M_PROJECT_H_ */
