/*
 * m_shuffle_lower_test.cpp
 *
 *  Created on: 2014-11-11
 *      Author: casa
 */

#include "../../src/physical/m_shuffle_lower.h"
#include "../../src/physical/m_shuffle_upper.h"
#include "../../src/common/m_merger.h"

namespace physical {

int main_shuffle_2() {
	int nlower=2;
	int nupper=1;
	int port1=1001;
	Merger *merger1=new Merger(nlower, port1);

	int port2_1=1003;
	int port3_1=1004;
	Sender *sender2=new Sender(port2_1);
	Sender *sender3=new Sender(port3_1);

	Sender **sender2_=new Sender*[nupper];
	sender2_[0]=sender2;
	Sender **sender3_=new Sender*[nupper];
	sender3_[0]=sender3;

	ShuffleUpper *upper1=new ShuffleUpper(merger1);
	ShuffleLower *lower2=new ShuffleLower(sender2_);
	ShuffleLower *lower3=new ShuffleLower(sender3_);

	/* upper is simulating the distributed shuffle. */


	return 0;
}

int main_shuffle_3() {
	int nlower=3;
	int nupper=2;
	int port1=1001;
	int port2=1002;
	Merger *merger1=new Merger(nlower, port1);
	Merger *merger2=new Merger(nlower, port2);

	int port3_1=1003;
	int port3_2=1004;
	int port4_1=1005;
	int port4_2=1006;
	int port5_1=1007;
	int port5_2=1008;
	Sender *sender3_1=new Sender(port3_1);
	Sender *sender3_2=new Sender(port3_2);
	Sender *sender4_1=new Sender(port4_1);
	Sender *sender4_2=new Sender(port4_2);
	Sender *sender5_1=new Sender(port5_1);
	Sender *sender5_2=new Sender(port5_2);

	Sender **sender3=new Sender*[nupper];
	sender3[0]=sender3_1;
	sender3[1]=sender3_2;
	Sender **sender4=new Sender*[nupper];
	sender4[0]=sender4_1;
	sender4[1]=sender4_2;
	Sender **sender5=new Sender*[nupper];
	sender5[0]=sender5_1;
	sender5[1]=sender5_2;

	ShuffleUpper *upper1=new ShuffleUpper(merger1);
	ShuffleUpper *upper2=new ShuffleUpper(merger2);
	ShuffleLower *lower3=new ShuffleLower(sender3);
	ShuffleLower *lower4=new ShuffleLower(sender4);
	ShuffleLower *lower5=new ShuffleLower(sender5);

	/* upper is simulating the distributed shuffle. */

	return 0;
}

}
