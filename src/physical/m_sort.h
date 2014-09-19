/*
 * m_sort.h
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#ifndef M_SORT_H_
#define M_SORT_H_

#include "../expressions/m_sort_order.h"
#include "../common/m_configuration.h"
#include "../common/m_tree_node.h"
#include "../common/m_schema.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

/* sort iterator is designed to support internal sort and external sort. */
class Sort: public UnaryNode, public QueryPlan {
public:
	/*
	 *  when global is true, sort is on the reduce side and merge the sorted files.
	 *  when global is false, sort is on the map side and sort stream by using external way.
	 *   */
	Sort(vector<SortOrder *> expressions, QueryPlan *child, bool global);
	virtual ~Sort();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output();

private:
	vector<SortOrder *> expressions_;
	QueryPlan *child_;
	bool global_;

	Block *buffer_;

	Schema *schema_;
};

}

#endif /* M_SORT_H_ */
