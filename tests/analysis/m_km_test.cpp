/*
 * m_km_test.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: Casa
 */

#include "../../src/analysis/m_km.h"
#include "../../src/analysis/m_par_strategy.h"

int main_km() {
	KM *km=new KM();
//	km->eval();
//	km->LM();

	ParStrategy *ps=new ParStrategy();
	ps->eval();
	return 0;
}
