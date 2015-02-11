/*
 * m_shuffle_upper1.h
 *
 *  Created on: Feb 8, 2015
 *      Author: Casa
 */

#ifndef M_SHUFFLE_UPPER1_H_
#define M_SHUFFLE_UPPER1_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_tree_node.h"
#include "../common/m_merger.h"
#include "m_shuffle_lower.h"
#include "m_shuffle_lower1.h"
#include "m_query_plan.h"

#include <vector>
using namespace std;

namespace physical {

class ShuffleUpper1SerObj {
public:
	ShuffleUpper1SerObj(NewSchema, vector<string>, vector<string>, QueryPlan *, int);
	virtual ~ShuffleUpper1SerObj();

	ShuffleUpper1SerObj(){};

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

class ShuffleUpper1: public UnaryNode, public QueryPlan {
public:
	ShuffleUpper1(vector<Expression *> expressions, QueryPlan *child);
	ShuffleUpper1(ShuffleUpper1SerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	ShuffleUpper1(Merger *);
	ShuffleUpper1(){};
	virtual ~ShuffleUpper1();


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

	ShuffleUpper1SerObj *shuffle_ser_obj_;
private:
	bool gather_all();
	bool send_gather();
	bool serialization();

	static void *receive_route(void *);

private:
	Merger *merger_;

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
#endif /* M_SHUFFLE_UPPER1_H_ */
