/*
 * m_sender.h
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#ifndef M_SENDER_H_
#define M_SENDER_H_

#include "../common/m_logging.h"

#include <arpa/inet.h>
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
	virtual ~Sender();

	bool m_socket(string, int);
	bool m_send(void *);

private:
	int fd_;
};

#endif /* M_SENDER_H_ */
