/*
 * m_arithmetic.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_arithmetic.h"

#include <iostream>
using namespace std;

ADD::ADD(Expression *left, Expression *right)
:left(left),right(right) {

}

ADD::~ADD() {

}

void* add_int(void *l, void *r) {
	void *res;
	*(int *)res=*(int *)l+*(int *)r;
	return res;
};

void* add_double(void *l, void *r) {
	void *res;
	*(double *)res=*(double *)l+*(double *)r;
	return res;
}

void ADD::initilize() {
	add[t_int]=&add_int;
	add[t_double]=&add_double;
	left->initilize();
	right->initilize();
	return_type=TypeCast::getInstance()->TypePromotion[left->return_type][right->return_type];
	val=malloc(8);
}

void* ADD::eval() {
	void *val_Left,*val_right;
	TypeCast::getInstance()->type_cast[left->return_type][return_type](left->eval(),val);
	TypeCast::getInstance()->type_cast[right->return_type][return_type](right->eval(),val_right);
	return add[return_type](val_Left,val_right);
}
