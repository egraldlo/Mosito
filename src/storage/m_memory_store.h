/*
 * m_memory_store.h
 *
 *  Created on: Jan 18, 2015
 *      Author: Casa
 */

#ifndef M_MEMORY_STORE_H_
#define M_MEMORY_STORE_H_

#include "../common/m_buffer.h"

#include <vector>
using namespace std;

class MemoryStore {
public:
	MemoryStore();
	virtual ~MemoryStore();

	static MemoryStore *getInstance() {
		if(memstore_==0) {
			memstore_=new MemoryStore();
			return memstore_;
		}
		else {
			return memstore_;
		}
	}

	bool isCached(){
		if(blocks_.empty()) {
			return true;
		}
		else {
			return false;
		}
	}

	vector<Block *> blocks_;

private:
	static MemoryStore *memstore_;

};

#endif /* M_MEMORY_STORE_H_ */
