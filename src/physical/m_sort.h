/*
 * m_sort.h
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#ifndef M_SORT_H_
#define M_SORT_H_

#include "../../src/common/m_trees.h"
#include "../expressions/m_sort_order.h"
#include "../common/m_configuration.h"
#include "../common/m_synchronize.h"
#include "../common/m_tree_node.h"
#include "../common/m_logging.h"
#include "../common/m_schema.h"
#include "../common/m_timer.h"
#include "m_shuffle_upper1.h"
#include "m_query_plan.h"

#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

#include <stdio.h>

namespace physical {

#ifndef MULTI_PARTITION
typedef vector<void *> range;
#endif

#ifdef MULTI_PARTITION
struct range {
//public:
//	range();
//	~range();
	vector<void *> ranges;
//	pthread_mutex_t lock_;
	SpineLock *lock_;

	inline void put(void *tuple) {
//		pthread_mutex_lock(&lock_);
		lock_->acquire();
		ranges.push_back(tuple);
		lock_->release();
//		pthread_mutex_unlock(&lock_);
	}

};
#endif

class SortSerObj {
public:
	SortSerObj(NewSchema ns, QueryPlan *child)
	:ns_(ns), child_(child) {};
	~SortSerObj() {};

	SortSerObj(){};

	QueryPlan *child_;
	NewSchema ns_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & ns_ & child_;
	}
};

/* sort iterator is designed to support internal sort and external sort. */
class Sort: public UnaryNode, public QueryPlan {
public:
	/*
	 *  when global is true, sort is on the reduce side and merge the sorted files.
	 *  when global is false, sort is on the map side and sort stream by using external way.
	 *   */
	Sort(vector<SortOrder *> expressions, QueryPlan *child, bool global);
	Sort(SortSerObj *sort_ser_obj)
	:sort_ser_obj_(sort_ser_obj){};
	Sort(QueryPlan *);
	virtual ~Sort();

	Sort(){};

	bool prelude();
	bool execute(Block *);
	bool postlude();

	vector<Expression *> output();
	bool maxLast(void*, Schema *);

	NewSchema *newoutput();

	/* max the last tuple. */
	static bool compare(const void *left, const void *right);
	static void *single_sort(void *);

#ifndef MULTI_PARTITION
	void *heap_out();
#endif

	int compare_start_end(unsigned long value);

#ifdef MULTI_PARTITION
	static void *single_partition(void *);
public:
	struct Argument {
		Sort *pthis;
		int range;
	};
#endif

private:
	SortSerObj *sort_ser_obj_;

private:
	vector<SortOrder *> expressions_;
	QueryPlan *child_;
	Schema *schema_;
	bool global_;

	Block *buffer_;

	Heap *heap_;
	LoserTree *lt_;

	FILE** files_;
	Block** lt_buffer_;
	BufferIterator** lt_buffer_iterator_;

	int already_finish_;

private:
	NewSchema *ns_;

	vector<range> ranges_;

	pthread_t pths_[CPU_CORE];

	vector<Block *> blocks_;

	unsigned temp_cur_;

	vector<int> dist_ranges_;

public:
#ifndef SYN_JOIN
	int th;
#endif

private:
	unsigned count_;
	unsigned long long time_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<QueryPlan>(*this) & sort_ser_obj_;
	}
};

}

#endif /* M_SORT_H_ */
