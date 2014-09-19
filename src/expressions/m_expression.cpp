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

void Expression::display(void *attr) {
	val=attr;
	/*
	 * TODO: add a spliter ' | '.
	 * */
	if(return_type==t_int) {
		cout<<*(int *)val<<" | ";
	}
	if(return_type==t_long) {
		cout<<*(unsigned long*)val<<" | ";
	}
}
