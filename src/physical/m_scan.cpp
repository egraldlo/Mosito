/*
 * m_scan.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_scan.h"

namespace physical {

ScanSerObj::ScanSerObj(string file_path)
:file_path_(file_path) {

}

ScanSerObj::~ScanSerObj() {

}

Scan::Scan(vector<Expression* > expressions, ScanSerObj *scan_ser_obj)
:expressions_(expressions), scan_ser_obj_(scan_ser_obj){
	//TODO: this expression can be merged into scan_ser_obj
}

Scan::~Scan() {

}

bool Scan::prelude() {
	/* TODO: output_ must be compute. */
#ifndef SINGLE_NODE_TEST
	splits_stream_=fopen(scan_ser_obj_->file_path_.c_str(),"rb");
	buffer_=new char[BLOCK_SIZE];
#endif

#ifdef SINGLE_NODE_TEST
	/* read the configuration from configuration file. */
	filename_<<scan_ser_obj_->file_path_<<"."<<Configuration::getInstance()->get_worker_ip().c_str();
	/* for directly read from file. */
//	filename_<<scan_ser_obj_->file_path_;

	splits_stream_=fopen(filename_.str().c_str(),"rb");
	buffer_=new char[BLOCK_SIZE];
#endif
	startTimer(&tm_);
	int size;
	blocks bs;
	while((size=fread(buffer_,1,BLOCK_SIZE,splits_stream_))!=0) {
		Block *block=new Block(BLOCK_SIZE);
		block->storeBlock(buffer_,size);
		bs.push_back(block);
	}
	MemoryStore::getInstance()->blocks_.insert(make_pair(filename_.str(), bs));
	cout<<filename_.str().c_str()<<" the time spend is: "<<getSecond(tm_)<<endl;
	cursor_=0;
//	sleep(2);
	iscached_=MemoryStore::getInstance()->isCached();
	return true;
}

bool Scan::execute(Block *block) {
//	if(iscached_) {
//		int size=0;
//		if((size=fread(buffer_,1,BLOCK_SIZE,splits_stream_))!=0) {
//			block->storeBlock(buffer_,size);
//			MemoryStore::getInstance()->blocks_.push_back(block);
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//	else {
//		if(cursor_<MemoryStore::getInstance()->blocks_.size()) {
//			/* todo: BLOCK_SIZE is not the good way. */
//			block->storeBlock(MemoryStore::getInstance()->blocks_[cursor_++], BLOCK_SIZE);
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
	if(cursor_<MemoryStore::getInstance()->blocks_[filename_.str()].size()) {
		/* todo: BLOCK_SIZE is not the good way. */
		block->storeBlock(MemoryStore::getInstance()->blocks_[filename_.str()][cursor_]->getAddr(), BLOCK_SIZE);
		MemoryStore::getInstance()->blocks_[filename_.str()][cursor_++]->~Block();
//		block=MemoryStore::getInstance()->blocks_[cursor_++];
		return true;
	}
	else {
		return false;
	}
}

bool Scan::postlude() {
	fclose(splits_stream_);
	return true;
}

vector<Expression *> Scan::output() {
	return expressions_;
}

#ifdef EXPERIMENT_TEST
NewSchema *Scan::newoutput() {
	return &vd_;
}
#endif

#ifdef EXPERIMENT
	Scan::Scan(NewSchema vd, ScanSerObj *sso)
	:vd_(vd), scan_ser_obj_(sso) {

	}

#endif

}
