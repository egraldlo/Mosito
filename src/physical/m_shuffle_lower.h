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

class ShuffleLowerSerObj {
public:
	ShuffleLowerSerObj(NewSchema, vector<int>, QueryPlan*);
	virtual ~ShuffleLowerSerObj();

	/* needed constructor function. */
	ShuffleLowerSerObj(){};

public:
	NewSchema ns_;
	vector<int> seqs_;
	QueryPlan* child_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & seqs_ & child_;
	}

};

class ShuffleLower: public UnaryNode, public QueryPlan{
public:
	ShuffleLower(vector<Expression *> expressions, QueryPlan *child);
	ShuffleLower(ShuffleLowerSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	ShuffleLower(Sender **);
	ShuffleLower(){};
	virtual ~ShuffleLower();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput(){};
	vector<Expression *> output(){};

private:
	Sender **senders_;
	ShuffleLowerSerObj *shuffle_ser_obj_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}

#endif /* M_SHUFFLE_LOWER_H_ */
