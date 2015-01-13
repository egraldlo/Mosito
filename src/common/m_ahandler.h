/*
 * m_ahandler.h
 *
 *  Created on: Nov 29, 2014
 *      Author: Casa
 */

#ifndef M_AHANDLER_H_
#define M_AHANDLER_H_

#include "../common/m_message.h"

#include <string>
#include <iostream>
using namespace std;

/*
 * AHandler is a actor handler which can use c++ actor model lib
 * theron, so we call the Actor Handler.
 * */
class Thandler: public Theron::Actor {
public:
	Thandler(Theron::Framework &framework, const char *const name)
	:Theron::Actor(framework, name) {
		RegisterHandler(this, &Thandler::handler);
		RegisterHandler(this, &Thandler::handler1);
		RegisterHandler(this, &Thandler::handler2);
	}

private:
	void handler(const Message1 &message, const Theron::Address from) {
		cout<<"hello, the task is: "<<message.message<<endl;
	};

	void handler1(const MessageT &message, const Theron::Address from){
		cout<<"okokokokokokokooko: "<<message.mText<<endl;
	};

	void handler2(const MessageI &message, const Theron::Address from){
		cout<<"okokokokokokokooko: "<<message.c<<endl;
	};

};

#endif /* M_AHANDLER_H_ */
