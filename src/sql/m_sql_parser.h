/*
 * m_sql_parser.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SQL_PARSER_H_
#define M_SQL_PARSER_H_

#include "../../src/physical/m_query_plan.h"
#include "../../src/physical/m_debug.h"

#include "sql_node_struct.h"

#include <sys/time.h>

#include <vector>
#include <algorithm>
using namespace std;
using physical::QueryPlan;

extern "C" int yylex();
extern "C" int yyparse();

/* the variable in "sql.tab.cpp". */
extern Node* parsetreeroot;
/* the variable in "sql.tab.cpp". */
extern char globalInputText[10000];

class SqlParser {
public:
	SqlParser();
	virtual ~SqlParser();

	void execute();
	Node *getparsetreeroot();

	/*
	 * TODO: to support logical plan.
	 *  */
	QueryPlan *ast_2_lp(Node *);
	QueryPlan *osgwf_2_lp(Node *);
	QueryPlan *sgwf_2_lp(Node *){return 0;};
	QueryPlan *gwf_2_lp(Node *){return 0;};
	QueryPlan *wf_2_lp(Node *){return 0;};
	QueryPlan *f_2_lp(Node *){return 0;};

private:
	timeval finish_time_;
	timeval start_time_;

};

#endif /* M_SQL_PARSER_H_ */
