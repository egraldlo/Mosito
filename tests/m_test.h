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
#include "common/m_trees_test.cpp"
#include "physical/m_debug_test.cpp"
#include "physical/m_sort_test.cpp"
#include "physical/m_scan_test.cpp"
#include "physical/m_join_test.cpp"
#include "sql/m_sql_parser_test.cpp"
#include "common/m_synchronize_test.cpp"
#include "common/m_sender_test.cpp"
#include "analysis/m_km_test.cpp"
#include "deploy/m_coordinator_test.cpp"
#include "physical/m_shuffle_lower_test.cpp"

#include <arpa/inet.h>

class Test {
public:
	Test();
	virtual ~Test();
};

int main(int argc, char** argv) {
//	TestThreadPool *tp=new TestThreadPool();
//	tp->test();
//	tp->test_circle();
//
//	Literal *a1=new Literal();
//	Literal *a2=new Literal();
//	int i=1;
//	double j=2.4;
//	a1->val=&i;
//	a1->return_type=t_int;
//	a2->val=&j;
//	a2->return_type=t_double;
//
//	TypeCast::getInstance()->initialize();
//	ADD *ad=new ADD(a1,a2);
//	ad->initilize();
//
//	Literal *a3=new Literal();
//	double k=2.8;
//	a3->val=&k;
//	a3->return_type=t_double;
//
//	Literal *a4=new Literal();
//	int l=2;
//	a3->val=&l;
//	a3->return_type=t_int;
//
//	ADD *addd=new ADD(ad,a4);
//	addd->initilize();
//	cout<<*(double *)ad->eval()<<endl;

//	m_losertrees_test();
//	m_heap_test();

	string path="table.left";
	physical::debug_test(path);

	/* for sort test. */
//	string path="table.left";
//	physical::sort_test(path);

	/* for lock test. */
//	main_lock();
//	main_rwlock();

	/* for KM algorithm test. */
//	main_km();

	/* for join test. */
//	string path1="table.left";
//	string path2="table.right";
//	physical::join_test(path1, path2);

	/* for scan test. */
//	scan_test();

	/* for sender test by socket. */
//	sender_local_test(argc, argv);

	/* for deploy test. */
//	main_coordinator(argc, argv);

	/* for shuffle 2 levels and shuffle 3 levels test. */
//	physical::main_shuffle_2();
//	physical::main_shuffle_3();

//	sql();
	return 0;
}

#endif /* M_TEST_H_ */
