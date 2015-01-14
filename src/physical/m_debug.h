/*
 * m_debug.h
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#ifndef M_DEBUG_H_
#define M_DEBUG_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_configuration.h"
#include "../common/m_schema.h"
#include "m_query_plan.h"

#include <stdio.h>

#include <vector>
using namespace std;

namespace physical {

class Debug: public UnaryNode, public QueryPlan {
public:
	Debug(vector<Expression *> expressions, QueryPlan *child);
	virtual ~Debug();

#ifdef EXPERIMENT
	Debug(QueryPlan *child);
#endif

	Debug() {};

	bool prelude();
	bool execute(Block *);
	bool postlude();

	void display(void *);
	void print(data_type,void *);

	NewSchema *newoutput(){};
	vector<Expression *> output();

private:
	vector<Expression *> expressions_;

	QueryPlan *child_;

	Block *buffer_;

	Schema *schema_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		/* child_ can be put in a serialized object. */
		ar & boost::serialization::base_object<QueryPlan>(*this) & child_;
	}
};

}

#endif /* M_DEBUG_H_ */
