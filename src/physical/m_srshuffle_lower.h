/*
 * m_srshuffle_lower.h
 *
 *  Created on: Jan 21, 2015
 *      Author: Casa
 */

#ifndef M_SRSHUFFLE_LOWER_H_
#define M_SRSHUFFLE_LOWER_H_

#include "../common/m_tree_node.h"
#include "../common/m_pc_buffer.h"
#include "../common/m_sender.h"
#include "m_query_plan.h"

#include <vector>
#include <sstream>
using namespace std;

namespace physical {

class SRShuffleLowerSerObj {
public:
	SRShuffleLowerSerObj(NewSchema, vector<string>, QueryPlan*, int);
	virtual ~SRShuffleLowerSerObj();

	/* needed constructor function. */
	SRShuffleLowerSerObj(){};

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

class SRShuffleLower: public UnaryNode, public QueryPlan{
public:
	SRShuffleLower(vector<Expression *> expressions, QueryPlan *child);
	SRShuffleLower(SRShuffleLowerSerObj *shuffle_ser_obj)
	:shuffle_ser_obj_(shuffle_ser_obj){};
	SRShuffleLower(Sender **);
	SRShuffleLower(){};
	virtual ~SRShuffleLower();

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
	int compare_start_end(int);
	static void *send_route(void *);

private:
	Sender **senders_;
	SRShuffleLowerSerObj *shuffle_ser_obj_;

private:
	pthread_t send_p_;
	PCBuffer *pcbuffer_;
	Block *buffer_;

	int debug_count_;
	int meet_zero_;

	vector<int> ranges_;
	Block **blocks_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<QueryPlan>(*this) & shuffle_ser_obj_;
	}

};

}

#endif /* M_SRSHUFFLE_LOWER_H_ */
