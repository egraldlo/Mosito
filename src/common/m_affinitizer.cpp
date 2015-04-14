/*
 * m_affinitizer.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_affinitizer.h"

Affinitizer::Affinitizer(int totalcpus, int corespercpu)
:totalcpus_(totalcpus),corespercpu_(corespercpu) {

}

Affinitizer::~Affinitizer() {

}

void Affinitizer::affinitize(int threadid) {
	/* TODO: a good strategy to schedule the threads among cpu cores. */
	int offset=threadid;
	cpu_set_t mask;


	CPU_ZERO(&mask);
	CPU_SET(offset,&mask);

	int tid=syscall(SYS_gettid);

	if(sched_setaffinity(tid,sizeof(cpu_set_t),&mask)==-1){
		cout<<"affinitize cpu error!"<<endl;
	}
}
