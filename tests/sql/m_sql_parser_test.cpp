/*
 * m_sql_parser_test.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "../../src/sql/m_sql_parser.h"

int sql(){
	SqlParser *sp=new SqlParser();
	sp->execute();
	return 0;
}
