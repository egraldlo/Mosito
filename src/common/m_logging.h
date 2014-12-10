/*
 * m_logging.h
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#ifndef M_LOGGING_H_
#define M_LOGGING_H_

#include "m_configuration.h"

#include <iostream>
using namespace std;

enum log_type{trace, debug, error};

class Logging {
public:
	Logging(log_type);
	virtual ~Logging();

	static Logging *getInstance();

	void log(log_type, const char *);

private:
	static Logging *logger_;
	log_type type_;
	bool switch_;
};

#endif /* M_LOGGING_H_ */
