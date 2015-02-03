/*
 * m_histogram_test.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: Casa
 */

#include "../../src/analysis/m_histogram.h"

int main_his(const char * path) {
	Histogram *his=new Histogram();
	his->eval(path);
	Histogram1 *his1=new Histogram1();
	his1->pthreads_eval(path);
	return 0;
}
