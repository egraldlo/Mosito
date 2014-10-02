/*
 * m_join.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_JOIN_H_
#define M_JOIN_H_

#include "m_logical_plan.h"

namespace logical {

class Join: public LogicalPlan {
public:
	Join();
	virtual ~Join();
};

} /* namespace logical */
#endif /* M_JOIN_H_ */
