/*
 * m_data_type.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_DATA_TYPE_H_
#define M_DATA_TYPE_H_

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

enum data_type{t_int,t_double,t_long};

class DataType {
public:
	DataType(data_type dt):dt_(dt){};
	virtual ~DataType(){};

	DataType(){};

	virtual bool gt(void *, void *)=0;
	virtual bool ge(void *, void *)=0;
	virtual bool eq(void *, void *)=0;
	virtual bool ls(void *, void *)=0;
	virtual bool le(void *, void *)=0;

	data_type get_type();

protected:
	data_type dt_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {}
};

class IntegerType: public DataType {
public:
	IntegerType(data_type dt)
	:DataType(dt){};
	virtual ~IntegerType(){};

	IntegerType(){};

	inline bool gt(void *l, void *r) {
		return *(int *)l>*(int *)r;
	}

	inline bool ge(void *l, void *r) {
		return *(int *)l>=*(int *)r;
	}

	inline bool eq(void *l, void *r) {
		return *(int *)l==*(int *)r;
	}

	inline bool ls(void *l, void *r) {
		return *(int *)l<*(int *)r;
	}

	inline bool le(void *l, void *r) {
		return *(int *)l<=*(int *)r;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<DataType>(*this) & dt_ ;
	}
};

class UnLongType: public DataType {
public:
	UnLongType(data_type dt)
	:DataType(dt){};
	virtual ~UnLongType(){};

	UnLongType(){};

	inline bool gt(void *l, void *r) {
		return *(unsigned long *)l>*(unsigned long *)r;
	}

	inline bool ge(void *l, void *r) {
		return *(unsigned long *)l>=*(unsigned long *)r;
	}

	inline bool eq(void *l, void *r) {
		return *(unsigned long *)l==*(unsigned long *)r;
	}

	inline bool ls(void *l, void *r) {
		return *(unsigned long *)l<*(unsigned long *)r;
	}

	inline bool le(void *l, void *r) {
		return *(unsigned long *)l<=*(unsigned long *)r;
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<DataType>(*this) & dt_;
	}
};

#endif /* M_DATA_TYPE_H_ */
