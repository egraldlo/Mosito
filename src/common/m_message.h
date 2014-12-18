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
	explicit MessageT(const string &mesg) {
		mMessage[0]='\0';
		memcpy(mMessage, mesg.c_str(), 15);
	}
	char mMessage[15];
};

THERON_DECLARE_REGISTERED_MESSAGE(MessageT)

#endif /* M_MESSAGE_H_ */
