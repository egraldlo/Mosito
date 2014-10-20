/*
 * m_type_cast.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_type_cast.h"

TypeCast *TypeCast::type_cast_=0;

TypeCast::TypeCast() {
	// TODO Auto-generated constructor stub

}

TypeCast::~TypeCast() {
	// TODO Auto-generated destructor stub
}

TypeCast* TypeCast::getInstance() {
	if(type_cast_==0){
		type_cast_=new TypeCast();
	}
	return type_cast_;
}

void int_to_int(void *src, void *des) {
	des=src;
}

void int_to_double(void *src, void *des){
	double *temp=reinterpret_cast<double *>(src);
	*(double *)des=*temp;
//	*(double* )des=*(int *)src;
}

void double_to_double(void *src, void *des) {
	des=src;
}

void TypeCast::initialize() {
	TypePromotion[t_int][t_int]=t_int;
	TypePromotion[t_int][t_double]=t_double;

	TypePromotion[t_double][t_int]=t_double;
	TypePromotion[t_double][t_double]=t_double;


	type_cast[t_int][t_int]=&int_to_int;
	type_cast[t_int][t_double]=&int_to_double;

	type_cast[t_double][t_double]=&double_to_double;

}
