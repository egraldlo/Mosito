/*
 * m_catalog.h
 *
 *  Created on: 2014-10-2
 *      Author: casa
 */

#ifndef M_CATALOG_H_
#define M_CATALOG_H_

#include "../../src/expressions/m_expression.h"
#include "../logical/m_logical_plan.h"

#include <string>
#include <map>
using namespace std;

typedef vector<Column> Table;

class Catalog {
public:
	Catalog();
	virtual ~Catalog();

	bool registerRelation(string, Table);

	/* first is table name, second is alais. */
	logical::LogicalPlan lookupRelation(string, string);

private:
	map<string, Table> tables;
};

#endif /* M_CATALOG_H_ */
