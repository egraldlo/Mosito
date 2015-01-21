/*
 * m_bcshuffle_lower.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Casa
 */

#ifndef M_BCSHUFFLE_LOWER_H_
#define M_BCSHUFFLE_LOWER_H_

#include "../common/m_tree_node.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_sender.h"
#include "m_query_plan.h"

#include <vector>
#include <sstream>
using namespace std;

namespace physical {

class BCShuffleLowerSerObj {
public:
	BCShuffleLowerSerObj(NewSchema, vector<int>, QueryPlan*, int);
	virtual ~BCShuffleLowerSerObj();

	/* needed constructor function. */
	BCShuffleLowerSerObj(){};

public:
	NewSchema ns_;
	vector<int> seqs_;
	QueryPlan* child_;
	int exchange_id_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & seqs_ & child_ & exchange_id_;
	}

};

class BCShuffleLower: public UnaryNode, public QueryPlan{
public:
	BCShuffleLower(vector<Expression *> expressions, QueryPlan *child);
	BCShuffleLower(BCShuffleLowerSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	BCShuffleLower(Sender **);
	BCShuffleLower(){};
	virtual ~BCShuffleLower();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	NewSchema *newoutput(){};
	vector<Expression *> output(){};

	/*
	 * when sending the block out, we store the tuple count in the last of the block
	 *       copy from the Block to void *.
	 * todo: this function must be in block, because block is flexible, we can change
	 *       the model of block, modify the model in the block is ok.
	 * */
	bool seriliaze(Block *, void *);

private:
	static void *send_route(void *);

private:
	Sender **senders_;
	BCShuffleLowerSerObj *shuffle_ser_obj_;

private:
	pthread_t send_p_;
	PCBuffer *pcbuffer_;
	Block *buffer_;

	int debug_count_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}

#endif /* M_BCSHUFFLE_LOWER_H_ */
