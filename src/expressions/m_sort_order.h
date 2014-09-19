/*
 * m_sort_order.h
 *
 *  Created on: 2014-9-19
 *      Author: casa
 */

#ifndef M_SORT_ORDER_H_
#define M_SORT_ORDER_H_

#include "m_expression.h"

enum direction{ascending, descending};

class SortOrder {
public:
	SortOrder(Expression *, direction);
	virtual ~SortOrder();

	/* ugly code, not public member. */
	Expression *expression_;

private:
	direction direction_;
};

#endif /* M_SORT_ORDER_H_ */
