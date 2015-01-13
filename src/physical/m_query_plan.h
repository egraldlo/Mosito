/*
 * m_query_plan.h
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#ifndef M_QUERY_PLAN_H_
#define M_QUERY_PLAN_H_

#include "../common/m_tree_node.h"
#include "../expressions/m_expression.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace physical {

class QueryPlan {
public:
	QueryPlan();
	virtual ~QueryPlan();

	virtual bool prelude()=0;
	virtual bool execute(Block *)=0;
	virtual bool postlude()=0;

#ifdef EXPERIMENT_TEST
	virtual NewSchema* newoutput()=0;
#endif
	virtual vector<Expression *> output()=0;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {}
};

}

#endif /* M_QUERY_PLAN_H_ */
