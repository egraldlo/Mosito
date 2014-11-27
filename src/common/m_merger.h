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

class Merger {
public:
	Merger(int);
	virtual ~Merger();

	bool m_socket(int );
	bool m_accept();

	bool m_merge(void *);

private:
	int fd_; //fd on this node.
	int nlower_; //map side nodes' number.
	int* map_lower_; //map fd to map side nodes.
};

#endif /* M_MERGER_H_ */
