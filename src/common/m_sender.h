/*
 * m_sender.h
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#ifndef M_SENDER_H_
#define M_SENDER_H_

#include "../common/m_logging.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include <string>
using namespace std;

/*
 * in the sender side, we use a sender to send a block to a merger node, so
 * there are multiple senders in the ShuffleLower.
 *  */
class Sender {
public:
	Sender();
	/* for local test, need to enter port number. */
	Sender(int port);
	virtual ~Sender();

	bool m_connect(string);
	bool m_send(const char *);
	bool m_close();

private:
	int fd_;
	int port_;
};

#endif /* M_SENDER_H_ */
