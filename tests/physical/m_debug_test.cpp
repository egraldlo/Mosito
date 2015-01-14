/*
 * m_debug_test.cpp
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#include "../../src/physical/m_debug.h"
#include "../../src/physical/m_scan.h"
#include "../../src/physical/m_project.h"
#include "../../src/common/m_configuration.h"
#include "../../src/common/m_serialization.h"
#include "../../src/expressions/m_expression.h"
#include "../../src/physical/m_query_plan.h"

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <sstream>
using namespace std;

namespace physical {

#ifdef EXPERIMENT_TEST
int debug_test(string path) {
	DataType *e1=new UnLongType(t_long);
	DataType *e2=new IntegerType(t_int);
	DataType *e3=new IntegerType(t_int);
	DataType *e4=new IntegerType(t_int);
	DataType *e5=new IntegerType(t_int);
	DataType *e6=new IntegerType(t_int);
	vector<DataType *> ve;
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	ve.push_back(e6);

	ScanSerObj *scan_ser_obj=new ScanSerObj(path);
	QueryPlan *scan=new Scan(ve,scan_ser_obj);
	QueryPlan *debug=new Debug(scan);

	/* test serialization. */
	std::ostringstream os;
	boost::archive::text_oarchive oa(os);
	register_obj(oa);
	register_data(oa);
	oa<<debug;

	std::istringstream is(os.str());
	boost::archive::text_iarchive ia(is);
	register_obj(ia);
	register_data(ia);
	QueryPlan *qp_debug;
	ia>>qp_debug;
	/***********************/

//	Scan *ss=reinterpret_cast<Scan *>(qp_scan);
//	QueryPlan *project=new Project(ve,qp_scan);

//	QueryPlan *debug=new Debug(qp_scan);
	qp_debug->prelude();
	qp_debug->execute(0);
	qp_debug->postlude();

	return 0;
}
#endif

#ifndef EXPERIMENT_TEST
int debug_test(string path) {
	Expression *e1=new Column();
	e1->return_type=t_long;
	Expression *e2=new Column();
	e2->return_type=t_int;
	Expression *e3=new Column();
	e3->return_type=t_int;
	Expression *e4=new Column();
	e4->return_type=t_int;
	Expression *e5=new Column();
	e5->return_type=t_int;
	Expression *e6=new Column();
	e6->return_type=t_int;

	vector<Expression*> ve;
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	ve.push_back(e6);

	ScanSerObj *scan_ser_obj=new ScanSerObj(path);
	QueryPlan *scan=new Scan(ve,scan_ser_obj);

	/* test serialization. */
	std::ostringstream os;
	boost::archive::text_oarchive oa(os);
	register_obj(oa);
	oa<<scan;

	std::istringstream is(os.str());
	boost::archive::text_iarchive ia(is);
	register_obj(ia);
	QueryPlan *qp_scan;
	ia>>qp_scan;
	/***********************/

//	Scan *ss=reinterpret_cast<Scan *>(qp_scan);
	QueryPlan *project=new Project(ve,qp_scan);

	QueryPlan *debug=new Debug(ve,scan);
	debug->prelude();
	debug->execute(0);
	debug->postlude();

	return 0;
}
#endif

} /* namespace physical */
