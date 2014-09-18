/*
 * m_scan.cpp
 *
 *  Created on: 2014-9-12
 *      Author: casa
 */

#include "m_scan.h"

namespace physical {

ScanSerObj::ScanSerObj(string file_path)
:file_path_(file_path){

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
	splits_stream_=fopen(scan_ser_obj_->file_path_.c_str(),"rb");
	buffer_=new char[BLOCK_SIZE];
	return true;
}

bool Scan::execute(Block *block) {
	int size=0;
	if((size=fread(buffer_,1,BLOCK_SIZE,splits_stream_))!=0) {
		block->storeBlock(buffer_,size);
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

}
