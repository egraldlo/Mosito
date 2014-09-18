/*
 * m_configuration.h
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#ifndef M_CONFIGURATION_H_
#define M_CONFIGURATION_H_

/* provide INT_MIN, INT_MAX etc. */
#include <limits.h>

#define BLOCK_SIZE 64*1024

class Configuration {
public:
	Configuration();
	virtual ~Configuration();
};

#endif /* M_CONFIGURATION_H_ */
