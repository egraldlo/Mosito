/*
 * m_srshuffle_upper.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Casa
 */

#ifndef M_SRSHUFFLE_UPPER_H_
#define M_SRSHUFFLE_UPPER_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_tree_node.h"
#include "../common/m_merger.h"
#include "m_shuffle_lower.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class SRShuffleUpperSerObj {
public:
	SRShuffleUpperSerObj(NewSchema, vector<string>, vector<string>, QueryPlan *, int);
	virtual ~SRShuffleUpperSerObj();

	SRShuffleUpperSerObj(){};

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

class SRShuffleUpper: public UnaryNode, public QueryPlan {
public:
	SRShuffleUpper(vector<Expression *> expressions, QueryPlan *child);
	SRShuffleUpper(SRShuffleUpperSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	SRShuffleUpper(Merger *);
	SRShuffleUpper(){};
	virtual ~SRShuffleUpper();


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
	SRShuffleUpperSerObj *shuffle_ser_obj_;

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
#endif /* M_SRSHUFFLE_UPPER_H_ */
