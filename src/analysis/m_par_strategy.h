/*
 * m_par_strategy.h
 *
 *  Created on: Jan 13, 2015
 *      Author: Casa
 */

#ifndef M_PAR_STRATEGY_H_
#define M_PAR_STRATEGY_H_

#include <iomanip>
#include <iostream>
using namespace std;

#include <limits.h>

#define N 20
#define P 4

class ParStrategy {
public:
	ParStrategy();
	virtual ~ParStrategy();

	int variance(int, int);

	void eval();

private:
	int a_array[N];
	int b_array[N];

	int a_expect;
	int b_expect;

	int state[N][P];
};

#endif /* M_PAR_STRATEGY_H_ */
