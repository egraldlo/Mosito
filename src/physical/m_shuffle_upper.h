/*
 * m_shuffle_upper.h
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#ifndef M_SHUFFLE_UPPER_H_
#define M_SHUFFLE_UPPER_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_tree_node.h"
#include "../common/m_merger.h"
#include "m_shuffle_lower.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleUpperSerObj {
public:
	ShuffleUpperSerObj(NewSchema, vector<string>, vector<string>, QueryPlan *, int);
	virtual ~ShuffleUpperSerObj();

	ShuffleUpperSerObj(){};

public:
	NewSchema ns_;
	vector<string> upper_seqs_;
	vector<string> lower_seqs_;
	QueryPlan* child_;
	int exchange_id_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & upper_seqs_ & lower_seqs_ & child_ & exchange_id_;
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

	NewSchema *newoutput();

	vector<Expression *> output(){};

	/*
	 * when receive the block from the remote node, we only get the data
	 * todo: this function must be in block, because block is flexible, we can change
	 *       the model of block, modify the model in the block is ok.
	 * */
	bool deseriliaze();

private:
	bool serialization();

	static void *receive_route(void *);

private:
	Merger *merger_;
	ShuffleUpperSerObj *shuffle_ser_obj_;

private:
	pthread_t receive_p_;
	PCBuffer *pcbuffer_;

	Block *block_temp_;

private:
	int debug_count_;
	int meet_zero_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}
#endif /* M_SHUFFLE_UPPER_H_ */
