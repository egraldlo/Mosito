/*
 * m_ahandler.cpp
 *
 *  Created on: Nov 29, 2014
 *      Author: Casa
 */

#include "m_ahandler.h"

AHandler::AHandler() {

}

AHandler::~AHandler() {

}

AConnection::AConnection(const char *name)
:name_(name) {

}

AConnection::~AConnection() {

}

void AConnection::initialize(const char *ip) {
	endpoint_=new Theron::EndPoint(name_, ip);
	Theron::Framework *framework=new Theron::Framework(*endpoint_);
	ahandler_=new AHandler(*framework,name_);

}
