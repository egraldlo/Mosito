/*
 * m_scan_test.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "../../src/physical/m_scan.h"
#include "../../src/common/m_buffer.h"
#include "../../src/common/m_configuration.h"

namespace physical {

int scan_test(){
	Buffer *buffer=new Block(BLOCK_SIZE,0);
	Expression *e1;
	Expression *e2;
	Expression *e3;
	Expression *e4;
	Expression *e5;
	vector<Expression*> ve;
	ve.push_back(e1);
	ve.push_back(e2);
	ve.push_back(e3);
	ve.push_back(e4);
	ve.push_back(e5);
	string file_path="";
	ScanSerObj *scan_ser_obj=new ScanSerObj(file_path);
	Iterator *scan=new Scan(ve,scan_ser_obj);
	return 0;
}

} /* namespace physical */
