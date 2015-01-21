/*
 * m_bcshuffle_upper.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Casa
 */

#ifndef M_BCSHUFFLE_UPPER_H_
#define M_BCSHUFFLE_UPPER_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_tree_node.h"
#include "../common/m_merger.h"
#include "m_bcshuffle_lower.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class BCShuffleUpperSerObj {
public:
	BCShuffleUpperSerObj(NewSchema, vector<int>, vector<int>, QueryPlan *, int);
	virtual ~BCShuffleUpperSerObj();

	BCShuffleUpperSerObj(){};

public:
	NewSchema ns_;
	vector<int> upper_seqs_;
	vector<int> lower_seqs_;
	QueryPlan* child_;
	int exchange_id_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & upper_seqs_ & lower_seqs_ & child_ & exchange_id_;
	}
};

class BCShuffleUpper: public UnaryNode, public QueryPlan {
public:
	BCShuffleUpper(vector<Expression *> expressions, QueryPlan *child);
	BCShuffleUpper(BCShuffleUpperSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	BCShuffleUpper(Merger *);
	BCShuffleUpper(){};
	virtual ~BCShuffleUpper();


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
	BCShuffleUpperSerObj *shuffle_ser_obj_;

private:
	pthread_t receive_p_;
	PCBuffer *pcbuffer_;

	Block *block_temp_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}
#endif /* M_BCSHUFFLE_UPPER_H_ */
