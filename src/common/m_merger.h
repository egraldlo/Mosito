/*
 * m_merger.h
 *
 *  Created on: Nov 24, 2014
 *      Author: casa
 */

#ifndef M_MERGER_H_
#define M_MERGER_H_

#include "../common/m_logging.h"

#include <arpa/inet.h>
#include <string.h>

/*
 * Act as a server in the server-client mode.
 *  */
class Merger {
public:
	Merger(int);
	/* for local test, a port will needed. */
	Merger(int nlowers, int port);
	virtual ~Merger();

	bool m_socket();
	bool m_accept();
	bool m_receive(char *);
	bool m_single(char*);
	bool m_close();

	bool m_merge(void *);

private:
	int fd_; //fd on this node.
	int port_;
	int nlower_; //map side nodes' number.
	int* map_lower_; //map fd to map side nodes.
};

#endif /* M_MERGER_H_ */
