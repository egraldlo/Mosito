/*
 * m_expression.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_expression.h"

Expression::Expression() {

}

Expression::~Expression() {

}

void Expression::display() {
	if(return_type==t_int) {
		cout<<*(int *)val<<endl;
	}
	if(return_type==t_long) {
		cout<<*(unsigned long*)val<<endl;
	}
}
