/*
 * m_logging.h
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#ifndef M_LOGGING_H_
#define M_LOGGING_H_

#include <iostream>
using namespace std;

class Logging {
public:
	Logging();
	virtual ~Logging();

	static Logging *getInstance();

	void log(const char *);

private:
	static Logging *logger_;
};

#endif /* M_LOGGING_H_ */
