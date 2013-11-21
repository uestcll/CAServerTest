#include "CLEpoll.h"
#include "CLSocket.h"
#include <vector>
#include <stdio.h>
#include <string.h>

#define PORT 50020

using namespace std;

struct ClientInfo
{
	CLSocket* p_sock;
	char buf[1024];
	int index;
};
int main()
{

	try
	{
		char buf[1024];
		CLSocket* sock = new CLSocket(0,PORT,true);
		sock->setNonBlock();
		sock->BindSocket();
		sock->ListenSocket();

		CLEpoll* m_epoll = new CLEpoll(50);
		m_epoll->Register_ctl(EPOLL_CTL_ADD,sock->getSock(),sock->getSock(),0,EPOLLIN | EPOLLET);

		int nfds = 0;
		int i = 0;
		int fd = -1;
		vector<ClientInfo*>* vec = new vector<ClientInfo*>;
		int clientfd;

		while(true)
		{
			nfds = m_epoll->EpollWait();
			i = 0;

			while(i < nfds)
			{
				m_epoll->getEventFd(i,&fd);

				if(fd == sock->getSock())
				{
					cout<<"accept"<<endl;
					clientfd = sock->AcceptSocket();
					CLSocket* p_sock = new CLSocket(clientfd);
					p_sock->setNonBlock();
					m_epoll->Register_ctl(EPOLL_CTL_ADD,clientfd,clientfd,0,EPOLLIN | EPOLLET);
					ClientInfo* info = (ClientInfo*)malloc(sizeof(ClientInfo));
					info->p_sock = p_sock;
					memset(info->buf,0,1024);
					info->index = 0;

					vec->push_back(info);

				}

				else if(m_epoll->getEvents()[i].events & EPOLLIN)
				{
					CLSocket* p_sock = 0;
					ClientInfo* info = 0;

					int j = 0;
					while(j < vec->size())
					{
						info = vec->at(j);
						if(info->p_sock->getSock() == fd)
						{
							p_sock = info->p_sock;
							break;
						}
					}

					if(j == vec->size())
					{
						cout<<"epoll in error fd"<<endl;
						continue;
					}

					memset(buf,0,1024);

					while(true)
					{
						uint32_t readlen = 0;
						int len = p_sock->ReadSocket(1024,(uint8_t*)buf,&readlen);

						if(len < 0)
						{
							if(errno == EAGAIN)
								cout<<"read all"<<endl;

							break;
						}

						if(len == 0)
						{
							cout<<"connect closed "<<endl;
							delete p_sock;
							m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN);
							break;
						}

						if(len < 1024)
						{
							cout<<buf<<endl;

							memcpy(info->buf+info->index,buf,strlen(buf));
							info->index += strlen(buf);
							memset(buf,0,1024);
							
							int r =	m_epoll->Register_ctl(EPOLL_CTL_ADD,fd,fd,0,EPOLLOUT | EPOLLET);

							break;
						}

						if(len == 1024)
						{
							memset(buf,0,1024);
							continue;
						}


					}
				}

				else if(m_epoll->getEvents()[i].events & EPOLLOUT)
				{
					CLSocket* p_sock = 0;
					ClientInfo* info = 0;

					int j = 0;
					while(j < vec->size())
					{
						info = vec->at(j);
						if(info->p_sock->getSock() == fd)
						{
							p_sock = info->p_sock;
							break;
						}
					}
					if(j == vec->size())
					{
						cout<<"epoll out error fd"<<endl;
						continue;
					}

					int writelen = 0;
					if(info->index <= 0)
						continue;

					writelen = p_sock->WriteSocket((uint8_t*)info->buf,info->index);
					if(writelen < 0)
					{
						if(errno == EAGAIN || errno == EINTR )
						{
							cout<<"epoll out write buf full"<<endl;
							continue;
						}

					}

					if(writelen == 0)
					{
						cout<<"write 0 len"<<endl;
						continue;

					}
					if(writelen == info->index)
					{
						cout<<"epoll out write all"<<endl;
						memset(info->buf,0,1024);
						info->index = 0;
						int r =	m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLOUT | EPOLLET);
						cout<<"epoll out del"<<endl;
						continue;
					}

					if(writelen < info->index)
					{
						cout<<"epoll out write buf full"<<endl;
						continue;
					}

				}
				
			}
		}

	}
	catch(const char* str)
	{
		cout<<str<<endl;
	}

	return 0;
}