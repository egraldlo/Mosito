/*
 * m_join_test.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "../../src/physical/m_join.h"
#include "../../src/physical/m_sort.h"
#include "../../src/physical/m_scan.h"
#include "../../src/physical/m_debug.h"
#include "../../src/common/m_configuration.h"
#include "../../src/expressions/m_expression.h"
#include "../../src/physical/m_query_plan.h"

namespace physical {
int join_test(string left_path, string right_path) {
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

	ScanSerObj *scan_ser_obj=new ScanSerObj(left_path);
	QueryPlan *scan1=new Scan(ve,scan_ser_obj);
	QueryPlan *scan2=new Scan(ve,scan_ser_obj);

	SortOrder so1(e2,ascending);
	vector<SortOrder *> vs1;
	vs1.push_back(&so1);
	QueryPlan *sort1=new Sort(vs1,scan1,false);

	SortOrder so2(e2,ascending);
	vector<SortOrder *> vs2;
	vs2.push_back(&so2);
	QueryPlan *sort2=new Sort(vs2,scan2,false);

	vector<Expression*> veall;
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	ve.push_back(e6);
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	ve.push_back(e6);

	vector<Expression *> cond;
	MergeJoin *mj=new MergeJoin(ve, ve, cond, sort1, sort2, inner);

	QueryPlan *debug=new Debug(veall,mj);
	debug->prelude();
	debug->execute(0);
	debug->postlude();
	return 0;
}

int mergejoin_test() {
	string file="/home/Casa/git/Mosito/table.left";//30000
	string file1="/home/Casa/git/Mosito/table.right";//30000
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
	NewSchema ns(ve);

	vector<DataType *> ve1;
	ve1.push_back(e1);
	ve1.push_back(e2);
	ve1.push_back(e3);
	ve1.push_back(e4);
	ve1.push_back(e5);
	ve1.push_back(e6);
	ve1.push_back(e1);
	ve1.push_back(e2);
	ve1.push_back(e3);
	ve1.push_back(e4);
	ve1.push_back(e5);
	ve1.push_back(e6);
	NewSchema ns1(ve1);

	ScanSerObj *scan_ser_obj=new ScanSerObj(file);
	ScanSerObj *scan_ser_obj1=new ScanSerObj(file1);
	QueryPlan *scan=new Scan(ve,scan_ser_obj);
	QueryPlan *scan1=new Scan(ve, scan_ser_obj1);
	QueryPlan *sort=new Sort(scan);
	QueryPlan *sort1=new Sort(scan1);

	MergeJoinSerObj *mjso=new MergeJoinSerObj(ns, ns, ns1, sort, sort1);
	QueryPlan *mj=new MergeJoin(mjso);

	QueryPlan *debug=new Debug(mj);

	debug->prelude();
	debug->execute(0);
	debug->postlude();

	return 0;
}

} /* namespace physical */
