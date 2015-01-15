/*
 * m_pc_buffer.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: Casa
 */

#include "m_pc_buffer.h"

PCBuffer::PCBuffer(NewSchema *ns)
:ns_(ns) {
	// TODO Auto-generated constructor stub

}

PCBuffer::~PCBuffer() {
	// TODO Auto-generated destructor stub
}

NewSchema *PCBuffer::getSchema() {
	return ns_;
}
