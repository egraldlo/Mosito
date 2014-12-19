/*
 * m_message.h
 *
 *  Created on: Dec 18, 2014
 *      Author: Casa
 */

#ifndef M_MESSAGE_H_
#define M_MESSAGE_H_

#include "../../third_party/theron/Theron/Defines.h"
#include "../../third_party/theron/Theron/Theron.h"

#include <string>
using namespace std;

/*
 * we must register the message in theron firstly, and we can use them
 * then, so every handler must have the own message.
 * */
struct MessageT {
	explicit MessageT(const char * const mesg) {
		mText[0]='\0';
		strcpy(mText, mesg);
	}
	char mText[256];
};

struct MessageI {
	explicit MessageI(int i) {
		c=i;
	}
	int c;
};

THERON_DECLARE_REGISTERED_MESSAGE(MessageT)
THERON_DECLARE_REGISTERED_MESSAGE(MessageI)

#endif /* M_MESSAGE_H_ */
