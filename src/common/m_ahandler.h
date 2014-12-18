/*
 * m_ahandler.h
 *
 *  Created on: Nov 29, 2014
 *      Author: Casa
 */

#ifndef M_AHANDLER_H_
#define M_AHANDLER_H_

//#include "../../third_party/theron/Theron/Defines.h"
//#include "../../third_party/theron/Theron/Theron.h"

#include <Theron/Defines.h>
#include <Theron/Theron.h>
#include "../common/m_logging.h"
#include "../common/m_message.h"

#include <string>
#include <iostream>
using namespace std;

/*
 * AHandler is a actor handler which can use c++ actor model lib
 * theron, so we call the Actor Handler.
 * */
class AConnection: public Theron::Actor {
public:
	AConnection(Theron::Framework &framework, const char *const name)
	:Theron::Actor(framework, name) {
		Logging::getInstance()->getInstance()->log(trace, "register a handler in actor!");
		RegisterHandler(this, &AConnection::handler1);
	}

private:
	void handler1(const MessageT &message, const Theron::Address from){
		cout<<"okokokokokokokooko"<<endl;
//		cout<<"fuck you!: "<<message.message<<endl;
	};
};

#endif /* M_AHANDLER_H_ */
