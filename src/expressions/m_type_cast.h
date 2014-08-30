/*
 * m_type_cast.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TYPE_CAST_H_
#define M_TYPE_CAST_H_

#include <string.h>
#include <malloc.h>

#include <iostream>
using namespace std;

#include "../common/m_data_type.h"

typedef void (*TypeCastFunc)(void *, void *);

class TypeCast {
public:
	TypeCast();
	virtual ~TypeCast();

	static TypeCast* getInstance();
	void initialize();

public:
    data_type TypePromotion[2][2];
	TypeCastFunc type_cast[2][2];

private:
	static TypeCast *type_cast_;
};

#endif /* M_TYPE_CAST_H_ */
