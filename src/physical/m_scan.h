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

#include "../../src/expressions/m_expression.h"
#include "../common/m_tree_node.h"
#include "../common/m_configuration.h"
#include "m_query_plan.h"

#include <stdio.h>

#include <vector>
#include <string>
using namespace std;

namespace physical {

class ScanSerObj {
public:
	ScanSerObj(string file_path);
	virtual ~ScanSerObj();

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
	virtual ~Scan();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	void display();

	vector<Expression *> output();

private:
	vector<Expression *> expressions_;
	ScanSerObj *scan_ser_obj_;
	FILE *splits_stream_;

	char *buffer_;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<QueryPlan>(*this)
				& expressions_ & scan_ser_obj_ ;
	}
};

}

#endif /* M_SCAN_H_ */
