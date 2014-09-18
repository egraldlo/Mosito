/*
 * m_debug.h
 *
 *  Created on: 2014-9-13
 *      Author: casa
 */

#ifndef M_DEBUG_H_
#define M_DEBUG_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_configuration.h"
#include "../common/m_schema.h"

#include <stdio.h>

#include <vector>
using namespace std;

namespace physical {

class Debug: public UnaryNode, public Iterator {
public:
	Debug(vector<Expression *> expressions, Iterator *child);
	virtual ~Debug();

	bool prelude();
	bool execute(Block *);
	bool postlude();

	void display(void *);

private:
	vector<Expression *> expressions_;
	Iterator *child_;

	Block *buffer_;

	Schema *schema_;
};

}

#endif /* M_DEBUG_H_ */
