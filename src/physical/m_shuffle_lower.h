/*
 * m_shuffle_lower.h
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#ifndef M_SHUFFLE_LOWER_H_
#define M_SHUFFLE_LOWER_H_

#include "../common/m_tree_node.h"
#include "../../src/common/m_sender.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleLower: public UnaryNode, public QueryPlan{
public:
	ShuffleLower(vector<Expression *> expressions, QueryPlan *child);
	ShuffleLower(Sender **);
	virtual ~ShuffleLower();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output(){};

private:
	Sender **senders_;

};

}

#endif /* M_SHUFFLE_LOWER_H_ */
