/*
 * m_serialization.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_SERIALIZATION_H_
#define M_SERIALIZATION_H_

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

//#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/stream.hpp>

//#include "../physical/m_scan.h"
//using namespace physical;

//#pragma auto_inline
template <class Archive>
void register_obj(Archive &ar);// {
//	ar.register_type(static_cast<Scan *>(NULL));
//}

#endif /* M_SERIALIZATION_H_ */
