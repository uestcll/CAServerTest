#include "CLEpoll.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

CLEpoll::CLEpoll(int maxsize /* = EPOLL_SIZE */)
{
	MaxSize = maxsize;
	epfd = epoll_create(MaxSize);
	if(epfd == -1)
		throw "In CLEpoll::CLEpoll(),epoll create error";

	ev = (struct epoll_event*)malloc(sizeof(struct epoll_event));
	events = (struct epoll_event*)malloc(MaxSize * sizeof(struct epoll_event));
	nfds = 0;
}	

CLEpoll::~CLEpoll()
{
	if(ev != 0)
	{
		free(ev);
		ev = 0;
	}
	if(events != 0)
	{
		free(events);
		events = 0;
	}
}

int CLEpoll::Register_ctl(int op,int fd,int eventfd,void* ptr,uint32_t events /* = EPOLLIN | EPOLLET */)
{
	ev->events = events;
	if(ptr == 0)
		ev->data.fd = fd;
	else if(eventfd < 0)
		ev->data.ptr = ptr;
	else
		ev->data.fd = fd;

	return epoll_ctl(epfd,op,fd,ev);
}

int CLEpoll::EpollWait(int timeout /* = EPOLL_TIME_OUT */)
{
	nfds = epoll_wait(epfd,events,MaxSize,timeout);

	if(nfds < 0)
		return -1;
	
	return nfds;
}

epoll_event* CLEpoll::getEvents()
{
	return events;
}

int CLEpoll::getEventFd(int index,int* fd)
{
	if(index >= nfds)
		return -1;

	*fd = events[index].data.fd;
	return 0;
}

int CLEpoll::getEventPtr(int index,void** ptr)
{
	if(index >= nfds)
		return -1;
	*ptr = events[index].data.ptr;
	return 0;
}