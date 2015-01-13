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
#include "../common/m_merger.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleUpper: public UnaryNode, public QueryPlan {
public:
	ShuffleUpper(vector<Expression *> expressions, QueryPlan *child);
	ShuffleUpper(Merger *);
	virtual ~ShuffleUpper();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput(){};
	vector<Expression *> output(){};

private:
	bool serialization();

private:
	Merger *merger_;
};

}
#endif /* M_SHUFFLE_UPPER_H_ */
