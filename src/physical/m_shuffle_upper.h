/*
 * m_shuffle_upper.h
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#ifndef M_SHUFFLE_UPPER_H_
#define M_SHUFFLE_UPPER_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_tree_node.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleUpper: public UnaryNode, public QueryPlan {
public:
	ShuffleUpper(vector<Expression *> expressions, QueryPlan *child);
	virtual ~ShuffleUpper();

	bool prelude();
	bool execute();
	bool postlude();

private:
	vector<Expression *> expressions_;
	QueryPlan *child_;

};

}
#endif /* M_SHUFFLE_UPPER_H_ */
