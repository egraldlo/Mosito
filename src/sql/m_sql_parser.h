/*
 * m_sql_parser.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SQL_PARSER_H_
#define M_SQL_PARSER_H_

class SqlParser {
public:
	SqlParser();
	virtual ~SqlParser();

public:
	void execute();
};

#endif /* M_SQL_PARSER_H_ */
