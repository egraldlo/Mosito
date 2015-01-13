/*
 * m_expression.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_EXPRESSION_H_
#define M_EXPRESSION_H_

#include "../common/m_data_type.h"
#include "../common/m_tree_node.h"

#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <stdlib.h>

#include <vector>
#include <iostream>
using namespace std;

class Column;

class Expression {
public:
	Expression();
	virtual ~Expression();

	virtual void* eval()=0;
	virtual void initilize()=0;
	virtual void display(void *);

public:
//protected:
	unsigned expr_id;
	data_type return_type;
	/*
	 * references will be a vector, because it can be presented as vector<column>.
	 * if "a+2" a belongs the references but 2 is not.
	 *  */
	vector<Column *> references;
	/*
	 * for evaluation.
	 * TODO: some good way.
	 *  */
	void *val;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & expr_id & return_type & references & val;
	}
};

class UnaryExpression: public Expression, public UnaryNode {
public:
	UnaryExpression(){};
	virtual ~UnaryExpression(){};

	virtual void initilize(){};
};

class BinaryExpression: public Expression, public BinaryNode {
public:
	BinaryExpression(){};
	virtual ~BinaryExpression(){};

	virtual void initilize(){};
};

class LeafExpression: public Expression, public LeafNode {
public:
	LeafExpression(){};
	virtual ~LeafExpression(){};

	virtual void initilize(){};

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<Expression>(*this);
	}
};

class Literal: public LeafExpression {
public:
	Literal(){};
	virtual ~Literal(){};

	virtual void initilize(){val=malloc(8);};
	void* eval(){return val;};
};

class Column: public LeafExpression {
public:
	Column(){};
	virtual ~Column(){};

	void *eval(){return 0;};
	void initilize(){};

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & boost::serialization::base_object<LeafExpression>(*this);
	}

};

#ifdef EXPERIMENT

class NewSchema {
public:
	NewSchema(vector<DataType *> &vd) {
		int offset=0;
		for(int i=0; i<vd.size(); i++) {
			if(vd.at(i)->get_type()==t_int) {
				vd_.push_back(new IntegerType(t_int));
				accum_.push_back(offset);
				offset+=4;
			}
			if(vd.at(i)->get_type()==t_long) {
				vd_.push_back(new UnLongType(t_long));
				accum_.push_back(offset);
				offset+=8;
			}
		}
		totalsize_=offset;
	};
	NewSchema(){};
	~NewSchema(){};

/* ugly but for experiment. */
public:
	unsigned totalsize_;
	/* not vector<DataType> but vector<DataType *> because of
	 * the DataType is a abstract class which can be serialized. */
	vector<DataType *> vd_;
	vector<int> accum_;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & totalsize_ & accum_ & vd_;
	}
};

#endif

#endif /* M_EXPRESSION_H_ */
