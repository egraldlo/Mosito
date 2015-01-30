/*
 * m_configuration.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_configuration.h"

Configuration *Configuration::configuration_=0;
#ifndef SINGLE_NODE_TEST
Configuration::Configuration() {
	const char *config_file="/home/Casa/git/Mosito/conf/master";
	try {
		cfg_.readFile(config_file);
	}
	catch(libconfig::FileIOException &e) {
		e.what();
	}

	initilize();
}

Configuration::~Configuration() {

}
#endif
bool Configuration::initilize() {
	coordinator_ip_=read_coordinator_ip();
	cout<<"coordinator_ip_: "<<coordinator_ip_<<endl;
	theron_worker_port_=read_theron_worker_port();
	cout<<"theron_worker_port_: "<<theron_worker_port_<<endl;
	worker_ip_=read_worker_ip();
	cout<<"worker_ip_: "<<worker_ip_<<endl;
}

string Configuration::read_coordinator_ip() {
	string ret;
	try {
		ret=(const char*)cfg_.lookup("COORDINATOR_IP");
		return ret;
	}
	catch(libconfig::SettingNotFoundException &e) {
		e.what();
	}
}

string Configuration::get_coordinator_ip() {
	return coordinator_ip_;
}

string Configuration::read_worker_ip() {
	string ret;
	try {
		ret=(const char*)cfg_.lookup("WORKER_IP");
		return ret;
	}
	catch(libconfig::SettingNotFoundException &e) {
		e.what();
	}
}

string Configuration::get_worker_ip() {
	return worker_ip_;
}

int Configuration::read_theron_worker_port() {
	int ret;
	try {
		ret=cfg_.lookup("THERON_WORKER_PORT");
		return ret;
	}
	catch(libconfig::SettingNotFoundException &e) {
		e.what();
	}
}

int Configuration::get_theron_worker_port() {
	return theron_worker_port_;
}
