/*
 * m_km.h
 *
 *  Created on: Dec 8, 2014
 *      Author: Casa
 */

#ifndef M_KM_H_
#define M_KM_H_

#include <stdio.h>
#include <string.h>

#define MAXN 310
#define INF 2147483647
#define max(x,y) ((x)>(y)?(x):(y))

class KM {
public:
	KM();
	virtual ~KM();

	int run();
	int eval();

	int LM();

private:
	bool search(int);
	bool find(int);

private:
	int n;
	int nx, ny;
	int w[MAXN][MAXN];
	int lx[MAXN],ly[MAXN];
	int link[MAXN];
	int visx[MAXN],visy[MAXN];
	int slack[MAXN];

	int map[MAXN][MAXN];
	int vis[MAXN];
	int link1[MAXN];
};

#endif /* M_KM_H_ */
