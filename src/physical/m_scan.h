/*
 * m_scan.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_SCAN_H_
#define M_SCAN_H_

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include "../expressions/m_expression.h"
#include "../storage/m_memory_store.h"
#include "../common/m_configuration.h"
#include "../common/m_tree_node.h"
#include "../common/m_timer.h"
#include "m_query_plan.h"

#include <stdio.h>

#include <vector>
#include <string>
#include <sstream>
using namespace std;

namespace physical {

class ScanSerObj {
public:
	ScanSerObj(string file_path);
	virtual ~ScanSerObj();

	/* needed constructor function. */
	ScanSerObj(){};

	string file_path_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & file_path_;
	}

};

class Scan :public LeafNode, public QueryPlan {
public:
	Scan(vector<Expression* > expressions, ScanSerObj *scan_ser_obj);

#ifdef EXPERIMENT
	Scan(NewSchema vd, ScanSerObj *scan_ser_obj);
#endif

	virtual ~Scan();

	/* needed constructor function. */
	Scan(){};

	bool prelude();
	bool execute(Block *);
	bool postlude();

	void display();

#ifdef EXPERIMENT_TEST
	NewSchema *newoutput();
#endif
	vector<Expression *> output();

private:
	/* expressions can be serialized. */
	vector<Expression *> expressions_;

	NewSchema vd_;
	ScanSerObj *scan_ser_obj_;

	FILE *splits_stream_;
	char *buffer_;

	unsigned cursor_;
	bool iscached_;

	unsigned long long tm_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<QueryPlan>(*this)
				& scan_ser_obj_ & vd_;
	}
};

}

#endif /* M_SCAN_H_ */
