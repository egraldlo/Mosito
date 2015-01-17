/*
 * m_configuration.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_configuration.h"

Configuration *Configuration::configuration_=0;

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

bool Configuration::initilize() {
	coordinator_ip_=read_coordinator_ip();
	theron_worker_port_=read_theron_worker_port();
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
