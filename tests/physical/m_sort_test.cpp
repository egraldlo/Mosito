/*
 * m_sort_test.cpp
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#include "../../src/physical/m_sort.h"
#include "../../src/physical/m_scan.h"
#include "../../src/physical/m_debug.h"
#include "../../src/common/m_configuration.h"
#include "../../src/expressions/m_expression.h"
#include "../../src/physical/m_query_plan.h"

namespace physical {

int sort_test(string path) {
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

	SortOrder so1(e2,ascending);
	vector<SortOrder *> vs;
	vs.push_back(&so1);
	QueryPlan *sort=new Sort(vs,scan,false);

	QueryPlan *debug=new Debug(ve,sort);
	debug->prelude();
	debug->execute(0);
	debug->postlude();

	return 0;
}

int sort_intern_test() {
//	string file="/home/Casa/git/Mosito/table.left.5567";//30000
	string file="/home/Casa/git/Mosito/table.left.1.98g";//30000*2340
	string file1="/home/Casa/git/Mosito/table.right.1.98g";//30000*2340
	ScanSerObj *scan_ser_obj=new ScanSerObj(file);
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


	QueryPlan *scan=new Scan(ve,scan_ser_obj);
	QueryPlan *sort=new Sort(scan);
	QueryPlan *debug=new Debug(sort);

	debug->prelude();
	debug->execute(0);
	debug->postlude();
//
//	ScanSerObj *scan_ser_obj1=new ScanSerObj(file1);
//	QueryPlan *scan1=new Scan(ve,scan_ser_obj1);
//	QueryPlan *sort1=new Sort(scan1);
//	QueryPlan *debug1=new Debug(sort1);
//
//	debug1->prelude();
//	debug1->execute(0);
//	debug1->postlude();

	return 0;
}

} /* namespace physical */
