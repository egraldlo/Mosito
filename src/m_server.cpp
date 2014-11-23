/*
 * m_server.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_server.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

Server::Server() {
	// TODO Auto-generated constructor stub

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

void Server::daemonize() {
	pid_t pid;
	struct rlimit rl;
	int fd0,fd1,fd2;
	/* 
	 * describe the action when signal comes. 
	 * */
	struct sigaction sa;

	/* 
	 * clear file creation mask. 
	 * */
	umask(0);

	if((pid=fork())<0)
		cout<<"error!"<<endl;
	else if(pid!=0)
		exit(0);

	setsid();

	/*
	 * ensure future opens won't allocate controlling TTYs.
	 * */
	sa.sa_handler=SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	if(sigaction(SIGHUP, &sa, 0)<0)
		cout<<"can't ignore the SIGHUP!"<<endl;

	if(chdir("/")<0)
		cout<<"change file failed."<<endl;

	/*
	 * get maximum number of file descriptors.
	 * */
	if(getrlimit(RLIMIT_NOFILE, &rl)<0) {
		cout<<"can't get the limit of the file descriptor!"<<endl;
	}

	/* 
	 * we use getrlimit to get the max open file descriptor, and close them which 
	 * get from the parent. so we can know that if we fork a process, we will get 
	 * all file descriptors from the parent.
	 * */
	if(rl.rlim_max==RLIM_INFINITY)
		rl.rlim_max=1024;
	for(int i=0;i<rl.rlim_max;i++){
		if(i==1)
			continue;
		else
			close(i);
	}	

	/* 
	 * make the /dev/null to point to file descriptor zero.
	 * */
	fd0=open("/dev/null",O_RDWR);
	/* 
	 * return a file descriptor which is the lowest of the 
	 * file system.
	 * */
	fd1=dup(0);
	fd2=dup(0);

	int fd=open("/home/casa/hellohahah.txt",O_WRONLY|O_CREAT);
	const char *ch="hello world!";
	write(fd,ch,20);
	close(fd);
	//while(true){
	cout<<" hello, process: "<<getpid()<<": "<<fd<<" "<<fd0<<" "<<fd1<<" "<<fd2<<endl;
	//}

}
