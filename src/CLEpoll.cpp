#include "CLEpoll.h"
#include <stdlib.h>

CLEpoll::CLEpoll(int maxsize /* = EPOLL_SIZE */)
{
	MaxSize = maxsize;
	epfd = epoll_create(MaxSize);
	ev = (struct epoll_event*)malloc(sizeof(struct epoll_event));
	events = (struct epoll_event*)malloc(MaxSize * sizeof(struct epoll_event));
}

CLEpoll::~CLEpoll()
{
	free(ev);
	free(events);
}

int CLEpoll::Register_ctl(int op,int fd,uint32_t events /* = EPOLLIN | EPOLLET */)
{
	ev->events = events;
	ev->data.fd = fd;
	return epoll_ctl(epfd,op,fd,ev);
}

int CLEpoll::EpollWait(int timeout /* = EPOLL_TIME_OUT */)
{
	return epoll_wait(epfd,events,MaxSize,timeout);
}

epoll_event* CLEpoll::getEvents()
{
	return events;
}

int CLEpoll::getEventFd(int index)
{
	return events[index].data.fd;
}