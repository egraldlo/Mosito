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

} /* namespace physical */
