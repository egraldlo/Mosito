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

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>

#include "../common/m_data_type.h"
#include "../physical/m_scan.h"
#include "../physical/m_sort.h"
#include "../physical/m_join.h"
#include "../physical/m_debug.h"
#include "../physical/m_shuffle_upper.h"
#include "../physical/m_shuffle_upper1.h"
#include "../physical/m_shuffle_lower.h"
using namespace physical;

#pragma auto_inline
template <class Archive>
void register_data(Archive &ar) {
	ar.register_type(static_cast<IntegerType *>(NULL));
	ar.register_type(static_cast<UnLongType *>(NULL));
}

#pragma auto_inline
template <class Archive>
void register_obj(Archive &ar) {
	ar.register_type(static_cast<Scan *>(NULL));
	ar.register_type(static_cast<Sort *>(NULL));
	ar.register_type(static_cast<Debug *>(NULL));
	ar.register_type(static_cast<MergeJoin *>(NULL));
	ar.register_type(static_cast<ShuffleUpper *>(NULL));
	ar.register_type(static_cast<ShuffleUpper1 *>(NULL));
	ar.register_type(static_cast<ShuffleLower *>(NULL));
}

#endif /* M_SERIALIZATION_H_ */
