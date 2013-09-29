#include "CLCAServerByEpoll.h"
#include "CLEpoll.h"
#include "CLCAClientContext.h"
#include "CLDataReceviverBySocket.h"
#include <iostream>

using namespace std;

CLCAServerByEpoll::CLCAServerByEpoll(uint8_t* IP,uint16_t Port,int IPType /* = AF_INET */,int streamType /* = SOCK_STREAM */):CLCAServer(IP,Port,IPType,streamType)
{
	
}

CLCAServerByEpoll::~CLCAServerByEpoll()
{

}

int CLCAServerByEpoll::Accept()
{

}

void CLCAServerByEpoll::Initialize()
{
	m_epoll = new CLEpoll();
	m_epoll->Register_ctl(EPOLL_CTL_ADD,m_sock->getSock());
	m_sock->setNonBlock();
	m_drev = new CLDataReceviverBySocket(m_sock);
	
}

void CLCAServerByEpoll::start()
{
	nfds = m_epoll->EpollWait();
}

list<CLCAClientContext*>* CLCAServerByEpoll::getData()
{
	list<CLCAClientContext*>* m_list = new list<CLCAClientContext*>;
	for(int i = 0;i<nfds ; i++)
	{
		if(m_epoll->getEventFd(i) == m_sock->getSock())
		{
			int clientFd = m_sock->AcceptSocket();
			CLSocket* sock = new CLSocket(clientFd);
			sock->setNonBlock();
			CLCAClientContext* client = new CLCAClientContext(sock);
		    client_lists->push_back(client);
		}
		else
		{
			CLCAClientContext* clientData = getClientData(m_epoll->getEventFd(i));
			if(clientData != 0 && clientData->isReadFull())
			{
				client_lists->remove(clientData);
				m_list->push_back(clientData);
				
			}

		}
	}

	return m_list;
}

CLCAClientContext* CLCAServerByEpoll::getClientData(int clientfd)
{
	 list<CLCAClientContext*>:: iterator it;
	 for(it = client_lists->begin();it != client_lists->end();it++)
	 {
		 if((*it)->isSock(clientfd))
			 break;
	 }

	 if(it == client_lists->end() && !(*it)->isSock(clientfd))
		 return 0;

	 m_drev->setContext(*it);
	return (CLCAClientContext*)(m_drev->getData());


	
	
}

int CLCAServerByEpoll::writeData(int sock,uint8_t* buf)
{
	return 0;
}

