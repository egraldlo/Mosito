/*
 * m_affinitizer.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_AFFINITIZER_H_
#define M_AFFINITIZER_H_

#include <sys/syscall.h>

#include <iostream>
using namespace std;

class Affinitizer {
public:
	/* TODO: consider numa framework. */
	Affinitizer(int totalcpus, int corespercpu);
	virtual ~Affinitizer();

	void affinitize(int threads);

private:
	int totalcpus_;
	int corespercpu_;

};

#endif /* M_AFFINITIZER_H_ */
