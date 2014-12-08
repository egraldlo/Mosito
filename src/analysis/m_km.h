/*
 * m_km.h
 *
 *  Created on: Dec 8, 2014
 *      Author: Casa
 */

#ifndef M_KM_H_
#define M_KM_H_

#include <cstdio>
#include <cmath>
using namespace std;

#include <string.h>

#define MAXN 20
#define INF 2147483647
#define max(x,y) ((x)>(y)?(x):(y))

class KM {
public:
	KM();
	virtual ~KM();

	int run();

private:
	bool search(int);

private:
	int w[MAXN][MAXN];
	int lx[MAXN],ly[MAXN];
	int linky[MAXN];
	int visx[MAXN],visy[MAXN];
	int lack;
};

#endif /* M_KM_H_ */
