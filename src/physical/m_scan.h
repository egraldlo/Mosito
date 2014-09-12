/*
 * m_scan.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_SCAN_H_
#define M_SCAN_H_

#include "../../src/expressions/m_expression.h"
#include "../common/m_tree_node.h"
#include "../common/m_configuration.h"

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

};

class Scan :public LeafNode, public Iterator {
public:
	Scan(vector<Expression* > expressions, ScanSerObj *scan_ser_obj);
	virtual ~Scan();

	bool prelude();
	bool execute(Block *);
	bool postlude();

private:
	/* here we can package the vector<Expression *> as schema */
	vector<Expression *> expressions_;
	ScanSerObj *scan_ser_obj_;
	FILE *splits_stream_;

	char *buffer_;
};

}

#endif /* M_SCAN_H_ */
