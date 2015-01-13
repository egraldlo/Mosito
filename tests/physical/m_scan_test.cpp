/*
 * m_scan_test.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "../../src/physical/m_scan.h"
#include "../../src/common/m_buffer.h"
#include "../../src/common/m_configuration.h"
#include "../../src/common/m_serialization.h"

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <sstream>
using namespace std;

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
	QueryPlan *scan=new Scan(ve,scan_ser_obj);

	std::ostringstream os;

	cout<<"hello world!"<<endl;

	boost::archive::text_oarchive oa(os);
	register_obj(oa);
	cout<<"hello world!"<<endl;
//	oa.register_type(static_cast<Scan *>(NULL));
	cout<<"hello world!"<<endl;
	oa<<scan;

	cout<<"after serialization: "<<os.str().c_str()<<endl;

	return 0;
}

} /* namespace physical */
