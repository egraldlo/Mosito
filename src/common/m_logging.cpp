/*
 * m_logging.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_logging.h"

Logging *Logging::logger_=0;

Logging::Logging() {

}

Logging::~Logging() {

}

Logging* Logging::getInstance() {
	if(logger_==0) {
		logger_=new Logging();
	}
	return logger_;
}

void Logging::log(const char *message) {
	cout<<message<<endl;
}
