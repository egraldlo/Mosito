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
#include "m_shuffle_lower.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleUpperSerObj {
public:
	ShuffleUpperSerObj(NewSchema, vector<int>, vector<int>, QueryPlan *);
	virtual ~ShuffleUpperSerObj();

	ShuffleUpperSerObj(){};

public:
	NewSchema ns_;
	vector<int> upper_seqs_;
	vector<int> lower_seqs_;
	QueryPlan* child_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & upper_seqs_ & lower_seqs_ & child_;
	}
};

class ShuffleUpper: public UnaryNode, public QueryPlan {
public:
	ShuffleUpper(vector<Expression *> expressions, QueryPlan *child);
	ShuffleUpper(ShuffleUpperSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	ShuffleUpper(Merger *);
	ShuffleUpper(){};
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
	ShuffleUpperSerObj *shuffle_ser_obj_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}
#endif /* M_SHUFFLE_UPPER_H_ */
