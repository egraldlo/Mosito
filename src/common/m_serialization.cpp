/*
 * m_serialization.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_serialization.h"

//#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/stream.hpp>
//
//#include "../physical/m_scan.h"
//using namespace physical;

//#pragma auto_inline
//template <class Archive>
//void register_obj(Archive &ar) {
//	ar.register_type(static_cast<Scan *>(NULL));
//}

//void cheat_the_compiler(){
//    char buffer[4096*2-sizeof(unsigned)];
//    boost::iostreams::basic_array_sink<char> sr(buffer, sizeof(buffer));
//    boost::iostreams::stream< boost::iostreams::basic_array_sink<char> > ostr(sr);
//    boost::archive::binary_oarchive oa(ostr);
//    boost::archive::text_oarchive toa(ostr);
//
//    register_obj<boost::archive::binary_oarchive>(oa);
//    register_obj<boost::archive::text_oarchive>(toa);
//    char a[2];
//    boost::iostreams::basic_array_source<char> device(a);
//    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
//    boost::archive::binary_iarchive ia(s);
//    boost::archive::text_iarchive tia(s);
//    register_obj<boost::archive::binary_iarchive>(ia);
//    register_obj<boost::archive::text_iarchive>(tia);
//}
