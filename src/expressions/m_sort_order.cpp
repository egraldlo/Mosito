/*
 * m_sort_order.cpp
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#include "m_sort_order.h"

SortOrder::SortOrder(Expression *expression, direction direction)
:expression_(expression),direction_(direction) {

}

SortOrder::~SortOrder() {

}
