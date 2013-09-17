#ifndef CLEPOLL_H
#define CLEPOLL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdint.h>

#define EPOLL_TIME_OUT -1
#define EPOLL_SIZE 10000

class CLEpoll
{
private:
	int epfd;
	struct epoll_event* events;
	struct epoll_event* ev;
	int MaxSize;

public:
	CLEpoll(int maxsize = EPOLL_SIZE);
	virtual ~CLEpoll();
	int Register_ctl(int op,int fd,uint32_t events = EPOLLIN | EPOLLET);
	int EpollWait(int timeout = EPOLL_TIME_OUT);
	epoll_event* getEvents();
	int getEventFd(int index);
};
#endif