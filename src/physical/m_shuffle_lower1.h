/*
 * m_shuffle_lower1.h
 *
 *  Created on: Feb 8, 2015
 *      Author: Casa
 */

#ifndef M_SHUFFLE_LOWER1_H_
#define M_SHUFFLE_LOWER1_H_

#include "../common/m_tree_node.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_sender.h"
#include "m_query_plan.h"

#include <vector>
#include <sstream>
using namespace std;

namespace physical {

class ShuffleLower1SerObj {
public:
	ShuffleLower1SerObj(NewSchema, vector<string>, QueryPlan*, int);
	virtual ~ShuffleLower1SerObj();

	/* needed constructor function. */
	ShuffleLower1SerObj(){};

public:
	NewSchema ns_;
	vector<string> seqs_;
	QueryPlan* child_;
	int exchange_id_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & seqs_ & child_ & exchange_id_;
	}

};

class ShuffleLower1: public UnaryNode, public QueryPlan{
public:
	ShuffleLower1(vector<Expression *> expressions, QueryPlan *child);
	ShuffleLower1(ShuffleLower1SerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	ShuffleLower1(Sender **);
	ShuffleLower1(){};
	virtual ~ShuffleLower1();

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
	int partition(void *, vector<int>);

private:
	Sender **senders_;
	ShuffleLower1SerObj *shuffle_ser_obj_;

private:
	pthread_t send_p_;
	PCBuffer *pcbuffer_;
	Block *buffer_;
	Block **buffers_;

	int debug_count_;
	int count_child_;
	int meet_zero_;

	vector<int> ranges_1_;
	vector<int> ranges_2_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}

#endif /* M_SHUFFLE_LOWER1_H_ */
