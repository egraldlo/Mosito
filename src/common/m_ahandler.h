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
#include <iostream>
using namespace std;

#define THERON_XS 1

/*
 * we must register the message in theron firstly, and we can use them
 * then, so every handler must have the own message.
 * */
struct Message1 {
	explicit Message1(const string &mesg) {
		message[0]='\0';
		memcpy(message, mesg.c_str(), 15);
	}
	char message[12];
};

/*
 * AHandler is a actor handler which can use c++ actor model lib
 * theron, so we call the Actor Handler.
 * */
class AConnection: public Theron::Actor {
public:
	AConnection(Theron::EndPoint *endpoint, Theron::Framework &framework, const char *const name)
	:endpoint_(endpoint), Theron::Actor(framework, name) {
		RegisterHandler(this, &AConnection::handler1);
	}
	virtual ~AConnection(){};

	void send() {
		framework_->Send(Message1("hello,world!"), Theron::Address(), Theron::Address("coordinator-addr"));
	};

private:
	void handler1(const Message1 &message, const Theron::Address from){
		cout<<"fuck you!: "<<message.message<<endl;
	};

private:
	Theron::Framework *framework_;
	Theron::EndPoint *endpoint_;
	Theron::Receiver reciever_;
};

#endif /* M_AHANDLER_H_ */
