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
	unsigned long start1=0;
	unsigned long end1=0;
	his->getbound(start1,end1);
	cout<<"start1: "<<start1<<"  end1:"<<end1<<endl;

	Histogram1 *his1=new Histogram1();
	his1->pthreads_eval(path);
	unsigned long start2=0;
	unsigned long end2=0;
	his1->getbound(start2,end2);
	cout<<"start2: "<<start2<<"  end2:"<<end2<<endl;

	return 0;
}
