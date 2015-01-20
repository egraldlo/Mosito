/*
 * m_logging.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "m_logging.h"

Logging *Logging::logger_=0;

Logging::Logging(log_type type) {
	type_=type;
}

Logging::~Logging() {

}

Logging* Logging::getInstance() {
	if(logger_==0) {
		logger_=new Logging(LOGGER);
	}
	return logger_;
}

void Logging::log(log_type type, const char *message) {
	/* determine which log level will be used. */
	if(type>=type_) {
		if(type==trace)
			cout<<"[trace]: "<<message<<endl;
		if(type==debug)
			cout<<"[debug]: "<<message<<endl;
		if(type==error)
			cout<<"[error]: "<<message<<endl;
	}
}
