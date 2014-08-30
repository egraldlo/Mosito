/*
 * m_test.h
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#ifndef M_TEST_H_
#define M_TEST_H_

#include "../src/expressions/m_arithmetic.h"
#include "../src/common/m_data_type.h"
#include "../src/common/m_smart_ptr.h"

#include "common/m_thread_pool_test.cpp"

#include <arpa/inet.h>

class Test {
public:
	Test();
	virtual ~Test();
};

int main() {
	TestThreadPool *tp=new TestThreadPool();
	tp->test();
	tp->test_circle();

	sleep(100);
	Literal *a1=new Literal();
	Literal *a2=new Literal();
	int i=1;
	double j=2.4;
	a1->val=&i;
	a1->return_type=t_int;
	a2->val=&j;
	a2->return_type=t_double;

	TypeCast::getInstance()->initialize();
	ADD *ad=new ADD(a1,a2);
	ad->initilize();

	Literal *a3=new Literal();
	double k=2.8;
	a3->val=&k;
	a3->return_type=t_double;

	Literal *a4=new Literal();
	int l=2;
	a3->val=&l;
	a3->return_type=t_int;

	ADD *addd=new ADD(ad,a4);
	addd->initilize();
	cout<<*(double *)ad->eval()<<endl;

	return 0;
}

#endif /* M_TEST_H_ */
