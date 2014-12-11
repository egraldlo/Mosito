/*
 * m_ahandler.h
 *
 *  Created on: Nov 29, 2014
 *      Author: Casa
 */

#ifndef M_AHANDLER_H_
#define M_AHANDLER_H_

#include "../../third_party/theron/Theron/Defines.h"
#include "../../third_party/theron/Theron/Theron.h"

#include <string>
using namespace std;

#define THERON_XS 1

/*
 * we must register the message in theron firstly, and we can use them
 * then, so every handler must have the own message.
 * */
struct Message1 {
	explicit Message1(const string &mesg) {
		message[0]='\0';
		memcpy(message, mesg.c_str(), 12);
	}
	char message[12];
};


/*
 * AHandler is a actor handler which can use c++ actor model lib
 * theron, so we call the Actor Handler.
 * */
class AHandler: public Theron::Actor {
public:
	AHandler(Theron::Framework &framework, const char *const name)
	:Theron::Actor(framework, name){
		RegisterHandler(this, &AHandler::handler1);
	}
	AHandler();
	virtual ~AHandler();

	bool ah_connect(){};

	bool ah_send(){};

private:
	void handler1(const Message1 &message, const Theron::Address from){};

private:
	Theron::Framework framework_;
	Theron::Receiver reciever_;
};

class AConnection {
public:
	AConnection(const char *name);
	virtual ~AConnection();

	void initialize(const char *);

protected:
	const char *name_;
	AHandler *ahandler_;
	Theron::EndPoint *endpoint_;

};

#endif /* M_AHANDLER_H_ */
