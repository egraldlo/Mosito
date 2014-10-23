/*
 * m_schema.cpp
 *
 *  Created on: 2014-9-18
 *      Author: casa
 */

#include "m_schema.h"

Schema::Schema() {

}

Schema::Schema(vector<Expression*> *v_e) {
	int offset=0;
	offset_=new int[v_e->size()];
	for(unsigned i=0;i<v_e->size();i++) {
		if(v_e->at(i)->return_type==t_int) {
			v_d_.push_back(new IntegerType());
			offset_[i]=offset;
			offset+=4;
		}
		if(v_e->at(i)->return_type==t_long) {
			v_d_.push_back(new IntegerType());
			offset_[i]=offset;
			offset+=8;
		}
	}
	bytes_=offset;
}

Schema::~Schema() {

}

int Schema::get_bytes() {
	return bytes_;
}

int Schema::get_columns() {
	return v_d_.size();
}
