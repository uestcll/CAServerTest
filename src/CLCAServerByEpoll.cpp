#include "CLCAServerByEpoll.h"
#include "CLEpoll.h"
#include "CLCAMessage.h"
#include "CLCAServerMsgProtocolDecapsulator.h"
#include "CLDataReceviver.h"
#include "CLDataReceviverBySocket.h"
#include "CLLogger.h"
#include "CLCADeSerializer.h"
#include "CLCASerializer.h"
#include "CLSocket.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

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

int CLCAServerByEpoll::ReTimingForReadClientFd(int fd)
{
	map<int,CLIENT_RECEVIVER_INFO*>::iterator it;
	it = map_ptdpl->find(fd);

	if(it == map_ptdpl->end())
		return -1;

	it->second->TimeStamp = 30;

	return 0;
}

int CLCAServerByEpoll::SetTimingForWriteClientFd(int fd)
{
	map<int,CLIENT_RECEVIVER_INFO*>::iterator it;
	it = map_ptdpl->find(fd);

	if(it == map_ptdpl->end())
		return -1;

	it->second->TimeStamp = 0;
	it->second->Logic_TimeStamp = 1;

	return 0;
}

void CLCAServerByEpoll::Initialize(CLDataReceviver* drecv)
{
	if( -1 == m_sock->setNonBlock())
		throw "In CLCAServerByEpoll::Initialize(),setNonBlock error";

	if( -1 == m_sock->BindSocket())
		throw "In CLCAServerByEpoll::Initialize(),BindSocket error";

	if( -1 == m_sock->ListenSocket(50))
		throw "In CLCAServerByEpoll::Initialize(),ListenSocket error";

	m_epoll = new CLEpoll();
	m_epoll->Register_ctl(EPOLL_CTL_ADD,m_sock->getSock(),m_sock->getSock(),0);
	
	m_drecv = drecv;
	//m_drecv = new CLDataReceviverBySocket;
	
}

void CLCAServerByEpoll::start()
{
#ifndef DEBUG_TEST
	CheckForClientFd();
#endif

	nfds = m_epoll->EpollWait();
}

void CLCAServerByEpoll::run()
{
	vector<CLIENT_CLBUFFER_INFO*>* client_Msg = new vector<CLIENT_CLBUFFER_INFO*>;
	CLIENT_CLBUFFER_INFO* ret = 0;
	map<int,CLIENT_RECEVIVER_INFO*> ::iterator it;

	for(int i = 0;i<nfds ; i++)
	{
		int fd  = -1;

		if( -1 == m_epoll->getEventFd(i,&fd))
		{
			CLLogger::WriteLogMsg("In CLCAServerByEpoll::getData(),getEventFd error",0);
			break;
		}

		if( fd == m_sock->getSock())
		{
			cout<<"Accepted"<<endl;
			int clientFd = 0;

			while(true)
			{
				clientFd = m_sock->AcceptSocket();
				
				if(clientFd == -1)
				{
					if(errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
						CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),m_sock->AcceptSocket error",0);
					
					break;
				}

				if(HandleAccept(clientFd) == -1)
				{
					CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),HandleAccept error",0);
					continue;
				}

			}

			continue;
		
		}

		if(m_epoll->getEvents()[i].events & EPOLLIN)
		{
			cout<<"GetData From client"<<endl;
			
			ret = getClientData(fd);

			if(ret != 0)
			{
				if(ret->buffer->size() == 0)
				{
					delete ret->buffer;
					free(ret);
					continue;
				}
				else
					client_Msg->push_back(ret);
			}
		}


		if(m_epoll->getEvents()[i].events & EPOLLOUT)
		{
			bool IsReadSet = true;
			map<int,CLIENT_RECEVIVER_INFO*>::iterator rev_it;
			rev_it = map_ptdpl->find(fd);

			if(rev_it == map_ptdpl->end())
			{
				IsReadSet = false;
			}
			else
			{
				CLIENT_RECEVIVER_INFO* rev_info = rev_it->second;
				if(rev_info == 0)
					IsReadSet = false;
				else
				{
					if(rev_info->IsDelete)
						IsReadSet = false;
				}
			}

			map<int,CLIENT_POST_INFO*>::iterator it;
			it = map_ptifo->find(fd);

			if(it == map_ptifo->end())
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),fd error",0);

				if(IsReadSet)
					m_epoll->Register_ctl(EPOLL_CTL_MOD,fd,fd,0,EPOLLIN | EPOLLET);
				else
					m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN | EPOLLOUT);

				continue;
			}

			

			CLIENT_POST_INFO* info = it->second;
			if(info == 0)
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),CLIENT_POST_INFO null",0);

				if(IsReadSet)
					m_epoll->Register_ctl(EPOLL_CTL_MOD,fd,fd,0,EPOLLIN | EPOLLET );
				else
					m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN | EPOLLOUT);


				continue;
			}

			info->IsDelete = false;

			if(info->buffer == 0)
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),buffer null",0);

				if(IsReadSet)
					m_epoll->Register_ctl(EPOLL_CTL_MOD,fd,fd,0,EPOLLIN | EPOLLET );
				else
					m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN | EPOLLOUT );

				info->IsDelete = true;
				continue;
			}

			uint32_t currentIovecIndex = info->buffer->getIovecIndex();

			if(-1 == info->buffer->BufferToIovec())
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),bufferToIovec error",0);

				if(IsReadSet)
					m_epoll->Register_ctl(EPOLL_CTL_MOD,fd,fd,0,EPOLLIN | EPOLLET );
				else
					m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN | EPOLLOUT );

				info->IsDelete = true;
				continue;

			}

			int len = info->m_sock->WritevSocket(info->buffer->getBufferToIovec(),
				info->buffer->getIovecCount());

			if(len < 0 )
			{
				///
				if(errno == EAGAIN || errno == EINTR )
				{
					continue;
				}
				else
				{
					CLLogger::WriteLogMsg("In CLCAServerByEpoll::run(),writevSocket error",0);
					FreeClientFd(fd);
					continue;
				}
			}

			if(len == 0)
			{
			//	FreeClientFd(fd);
				continue;
			}

			if(len == info->buffer->getBufferToIovecBufferSize())
			{
				delete info->buffer;
				info->buffer = 0;

				if(IsReadSet)
					m_epoll->Register_ctl(EPOLL_CTL_MOD,fd,fd,0,EPOLLIN | EPOLLET );
				else
				{
					m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLIN | EPOLLOUT);
					info->IsDelete = true;
				}
				continue;
			}

			if(len < info->buffer->getBufferToIovecBufferSize())
			{
				info->buffer->setIovecIndex(currentIovecIndex + len);
				continue;
			}

		}

		if(!((m_epoll->getEvents()[i].events & EPOLLOUT) || (m_epoll->getEvents()[i].events & EPOLLIN)))
		{
			fprintf(stderr,"%s","In CLCAServerByEpoll::getData(),epoll wrong");
		}
	}

	client_MsgBuf = client_Msg;
	client_Msg = 0;
}



CLIENT_MSG_INFO* CLCAServerByEpoll::getData()
{
	if(client_MsgBuf == 0 )
		return 0;

	if(client_MsgBuf->size() == 0)
	{
		delete client_MsgBuf;
		client_MsgBuf = 0;

		return 0;
	}

	CLIENT_MSG_INFO* ret_Msg_Info = 0;
	CLIENT_ONE_MSG_INFO* OneMsg_Info = 0;
	vector<CLIENT_ONE_MSG_INFO*>* ret_vec = new vector<CLIENT_ONE_MSG_INFO*>;

	vector<CLCAMessage*>* Msg_vec = 0;
	CLIENT_MSG_INFO* Msg_info = 0;
	// 获取 buffer对应的 消息类型
	vector<CLIENT_CLBUFFER_INFO*>::iterator it;
	vector<CLBuffer*>::iterator CLBuffer_it;
	map<uint32_t,CLCADeSerializer*>::iterator DeserIt;

	CLBuffer* buffer = 0;
	CLIENT_CLBUFFER_INFO* CLBuffer_info = 0;
	uint32_t MsgId = 0;
	uint8_t* buf = 0;
	bool IsDeleted = false;

	for( it = client_MsgBuf->begin(); it != client_MsgBuf->end(); it++)
	{
		CLBuffer_info = *it;

		for( CLBuffer_it = CLBuffer_info->buffer->begin();
			CLBuffer_it != CLBuffer_info->buffer->end();CLBuffer_it++)
		{
			buffer = *CLBuffer_it;
			buf = buffer->getBuf(0,4,&IsDeleted);
			MsgId = ntohl(*(uint32_t*)buf);
			
			if(IsDeleted)
				delete buf;

			DeserIt = map_DeSer->find(MsgId); 
			if(DeserIt == map_DeSer->end())
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::getData(),find CLCASerializer failed",0);
				delete buffer;
				continue;
			}
			
			Msg_vec = DeserIt->second->DeSerializer(buffer,8);
			
			delete buffer;
			
			if(Msg_vec == 0 )
				continue;

			if(Msg_vec->size() == 0)
			{
				delete Msg_vec;
				continue;
			}

			OneMsg_Info = (CLIENT_ONE_MSG_INFO*)malloc(sizeof(CLIENT_ONE_MSG_INFO));
			OneMsg_Info->sock = CLBuffer_info->sock;
			OneMsg_Info->Msg = Msg_vec;
			OneMsg_Info->MsgType = MsgId;

			ret_vec->push_back(OneMsg_Info);

		}

		delete CLBuffer_info->buffer;
		free(CLBuffer_info);

		CLBuffer_info = 0;
	}

	delete client_MsgBuf;
	client_MsgBuf = 0;

	if(ret_vec->size() == 0)
	{
		delete ret_vec;
		ret_vec = 0;
		return ret_Msg_Info;
	}

	ret_Msg_Info = (CLIENT_MSG_INFO*)malloc(sizeof(CLIENT_MSG_INFO));
	ret_Msg_Info->Msg = ret_vec;

	return ret_Msg_Info;

}

int CLCAServerByEpoll::HandleAccept(int clientfd)
{
	CLSocket* sock = new CLSocket(clientfd);

	if( -1 == sock->setNonBlock())
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::HandleAccept(),setNonBlock error",0);
	}

	if( -1 == m_epoll->Register_ctl(EPOLL_CTL_ADD,clientfd,clientfd,0,EPOLLIN | EPOLLET))
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::HandleAccept(),Register_ctl error",0);
		delete sock;
		close(clientfd);
		return -1;
	}

	if(map_ptdpl->find(clientfd) != map_ptdpl->end())
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::HandleAccept(),map<int,CLIENT_RECEVIVER_INFO*> find fd error",0);
		delete sock;
		return -1;
	}

	CLProtocolDecapsulator* ptdpl = new CLCAServerMsgProtocolDecapsulator;
	CLIENT_RECEVIVER_INFO* info = (CLIENT_RECEVIVER_INFO*)malloc(sizeof(struct CLIENT_RECEVIVER_INFO));
	info->m_sock = sock;
	info->ptdpl = ptdpl;
	info->IsDelete = false;
	info->Logic_TimeStamp = 0;
	ReTimingForReadClientFd(clientfd);

	map_ptdpl->insert(pair<int,CLIENT_RECEVIVER_INFO*>(clientfd,info));

	if(map_ptifo->find(clientfd) != map_ptifo->end())
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::HandleAccept(),map<int,CLIENT_POST_INFO*> find fd error",0);
		delete sock;
		return -1;
	}

	CLIENT_POST_INFO* post_info = (CLIENT_POST_INFO*)malloc(sizeof(CLIENT_POST_INFO));
	post_info->buffer = 0;
	post_info->m_sock = sock;
	post_info->IsDelete = true;

	map_ptifo->insert(pair<int,CLIENT_POST_INFO*>(clientfd,post_info));

	return 0;
}

CLIENT_CLBUFFER_INFO* CLCAServerByEpoll::getClientData(int clientfd)
{
	if(map_ptdpl->find(clientfd) == map_ptdpl->end())
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::getClientData(),map<int,CLProtocolDecapuslator> find clientfd failed",0);
		return 0;
	}

	CLIENT_RECEVIVER_INFO* info = map_ptdpl->find(clientfd)->second;
	info->IsDelete = false;
	ReTimingForReadClientFd(clientfd);

	CLSocket* sock = info->m_sock;
	CLProtocolDecapsulator* ptdpl = info->ptdpl;
	m_drecv->setContext(sock);

	uint32_t leftSize = 0;

	while(true)
	{
		leftSize = ptdpl->getLeftSize();
		uint8_t* buf = (uint8_t*)m_drecv->getData(leftSize);
		uint32_t len = m_drecv->getDataSize();

		if(len == 0)
		{
			if(buf != 0)
				delete buf;
		/////
			if( (m_drecv->geterrno() > 0) && (m_drecv ->geterrno() != EAGAIN) && (m_drecv->geterrno() != EINTR) )
			{
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::getClientData(), m_drecv ->getdata() error",0);
				FreeClientFd(clientfd);
			}
			//read调用返回0
			else if(m_drecv->geterrno() <= 0)
			{
				info->IsDelete  = true;
				SetTimingForWriteClientFd(clientfd);

				m_epoll->Register_ctl(EPOLL_CTL_MOD,clientfd,clientfd,0,EPOLLOUT | EPOLLET);
				
			}


			break;
		}

		ptdpl->addCharToDecapsulate(buf,len);
		ptdpl->ProtocolDecapsulate();

		if(ptdpl->getLeftSize() == 0)
		{
			// 解析完毕的处理 ....
			m_epoll->Register_ctl(EPOLL_CTL_MOD,clientfd,clientfd,0,EPOLLOUT | EPOLLET);
			info->IsDelete = true;
			SetTimingForWriteClientFd(clientfd);
		}
		else
			info->IsDelete = false;

		if(m_drecv ->IsReceviveAll())
			break;

	}
		
	CLIENT_CLBUFFER_INFO* CLBuffer_info = (CLIENT_CLBUFFER_INFO*)malloc(sizeof(CLIENT_CLBUFFER_INFO));
	CLBuffer_info->buffer = ptdpl->getDecapsulatorMsgChar();
	CLBuffer_info->sock = clientfd;
	
	if(CLBuffer_info->buffer == 0)
	{
		free(CLBuffer_info);
		return 0;
	}

	if(CLBuffer_info->buffer->size() == 0)
	{
		delete CLBuffer_info->buffer;
		free(CLBuffer_info);
		return 0 ;
	}

	return CLBuffer_info;

}

//该函数实现 将msg序列化 ，并通知epoll写
int CLCAServerByEpoll::writeData(vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId)
{
	if(msg_vec == 0 )
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::writeData(),msg_vec null",0);
		return -1;
	}

	if(msg_vec->size() == 0)
	{
		delete msg_vec;

		CLLogger::WriteLogMsg("In CLCAServerByEpoll::writeData(),msg_vec size is 0",0);
		return -1;
	}



	uint8_t* buf = 0;
	uint32_t buf_size = 0;
	CLCASerializer* ser = 0;

	
	vector<CLCAMessage*>::iterator msg_it;

	uint32_t MsgType = 0;
	map<int,CLIENT_POST_INFO*>::iterator post_info_it;

	post_info_it = map_ptifo->find(sock);

	if(post_info_it == map_ptifo->end())
	{
		for( msg_it = msg_vec->begin(); msg_it != msg_vec->end() ;msg_it ++)
		{
			if( (*msg_it) != 0)
				delete *msg_it;
		}

		delete msg_vec;
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::writeData(),sock not find",0);

		return -1;
	}


	CLIENT_POST_INFO* post_info = 0;
	CLBuffer* buffer = 0;
	CLCAMessage* msg = 0;

	post_info = post_info_it->second;
	if(post_info->buffer != 0)
		buffer = post_info->buffer;


	if(IsMutiMsg)
	{
		MsgType = MsgId;
		msg_it = msg_vec->begin();
	}
	//如果没有单报文多消息 则msgtype设定为第一条消息的类型
	else
	{
		msg_it = msg_vec->begin();
		msg = (*msg_it);
		
		if(msg != 0)
			MsgType = msg->m_MsgID;
	}
	
	map<uint32_t,CLCASerializer*>::iterator Ser_It;
	
	Ser_It = map_Ser->find(MsgType);

	if(Ser_It == map_Ser->end())
	{
		CLLogger::WriteLogMsg("In CLCAServerByEpoll::writeData(),CLCASerializer not find",0);
		
		//如果是多条消息序列化一条报文，没有找到序列化类 则 删除全部 。
		if(IsMutiMsg)  
		{
			for( msg_it = msg_vec->begin(); msg_it != msg_vec->end() ;msg_it ++)
			{
				msg = *msg_it;
				if( msg != 0)
					delete msg;
			}

			delete msg_vec;
			return -1;
		}
	   //否则直到找到一条有序列化类的消息
		else
		{
			do
			{ 

				msg_it ++ ;
				msg = (*msg_it);

				if(msg != 0)
					MsgType = msg->m_MsgID;
				else
					continue;

				Ser_It = map_Ser->find(MsgType);
				if(Ser_It != map_Ser->end())
					break;
			
			}while(msg_it != msg_vec->end());
		}

		if(msg_it == msg_vec->end())
		{
			for( msg_it = msg_vec->begin(); msg_it != msg_vec->end() ;msg_it ++)
			{
				msg = *msg_it;
				if( msg != 0)
					delete msg;
			}

			delete msg_vec;
			return -1;
		}
	}
	

	ser = Ser_It->second;

	if(buffer == 0)
	{
		buffer = new CLBuffer;
		post_info->buffer = buffer;
	}

	//若是单个的msg
	if(msg_vec->size() == 1 && !IsMutiMsg)
	{
		buf = ser->Serialize(msg,0,0,&buf_size,false);

		buffer->addBuf(buf,buf_size,false,CANDELETE);

	}
	else
	{

		uint32_t sum = 0;
		uint32_t SingleMsgType = 0;
		
		msg = *msg_it;

		if(msg != 0)
			SingleMsgType = msg->m_MsgID;
		else
			msg_it ++;

		vector<CLCAMessage*>* temp_vec = new vector<CLCAMessage*>;
		

		if(IsMutiMsg && (msg_it == msg_vec->begin()))
		{
			
			while(msg_it != msg_vec->end())
			{
				msg = (*msg_it);

				if(msg->m_MsgID != SingleMsgType)
				{
					if(sum == 0)
					{
						break; //这里sum 不可能为 0
					}

					buf = ser->Serialize(0,temp_vec,0,&buf_size,false);

					if(buf == 0)
					{
						CLLogger::WriteLogMsg("In CLCAServerByEpoll::WriteData(),ser->Serialize() error",0);

						sum = 0;
						break;
					}

					buffer->addBuf(buf,buf_size,false,CANDELETE);

					sum = 0;
						break;

				}
					temp_vec->push_back(msg);
				
					sum++;
					msg_it++;
			}
		}
		// sum == 0 表示并非全部都是同一类型msg，需要进行逐个序列化
		if(sum != 0)
		{
			buf = ser->Serialize(0,temp_vec,0,&buf_size,false);

			if(buf == 0)
				CLLogger::WriteLogMsg("In CLCAServerByEpoll::WriteData(),ser->Serialize() error",0);
			else
				buffer->addBuf(buf,buf_size,false,CANDELETE);
		}
		else
		{
			while(msg_it!= msg_vec->end())
			{
				msg = (*msg_it);

				if(msg != 0)
					MsgType = msg->m_MsgID;
				else
					continue;
		
				Ser_It = map_Ser->find(MsgType);
				
				if(Ser_It == map_Ser->end())
				{
					CLLogger::WriteLogMsg("In CLCAServerByEpoll::writeData(),CLCASerializer not find",0);
					continue;
				}

				ser = Ser_It->second;

				buf = ser->Serialize(msg,0,0,&buf_size,false);
			
				if(buf == 0)
				{
					CLLogger::WriteLogMsg("In CLCAServerByEpoll::WriteData(),ser->Serialize() error",0);
					continue;
				}

				buffer->addBuf(buf,buf_size,false,CANDELETE);


			}
		}

		delete temp_vec;

	}

	for( msg_it = msg_vec->begin(); msg_it != msg_vec->end() ;msg_it ++)
	{
		msg = *msg_it;

		if( msg != 0)
			delete msg;
	}

	delete msg_vec;

	post_info->IsDelete = false;
	
	map<int,CLIENT_RECEVIVER_INFO*>::iterator rev_it;
	rev_it = map_ptdpl->find(sock);

	if(rev_it != map_ptdpl->end())
	{
		CLIENT_RECEVIVER_INFO* rev_info = rev_it->second;

		if(rev_info == 0 || rev_info->IsDelete)
		{
				m_epoll->Register_ctl(EPOLL_CTL_MOD,sock,sock,0, EPOLLOUT | EPOLLET);

				return 0;
		}
		

	}
	else
	{
		m_epoll->Register_ctl(EPOLL_CTL_MOD,sock,sock,0, EPOLLOUT | EPOLLET);

		return 0;
	}

	m_epoll->Register_ctl(EPOLL_CTL_MOD,sock,sock,0,EPOLLIN | EPOLLOUT | EPOLLET);
	return 0;
}

//释放clientfd相关资源
void CLCAServerByEpoll::FreeClientFd(int clientfd)
{

	CLSocket* m_sock = 0;
	map<int,CLIENT_RECEVIVER_INFO*>::iterator recevive_it;
	
	recevive_it = map_ptdpl->find(clientfd);

	if(recevive_it != map_ptdpl->end())
	{
		CLIENT_RECEVIVER_INFO* recevive_info = recevive_it->second;
		if(recevive_info != 0)
		{
			m_sock = recevive_info->m_sock;

			CLProtocolDecapsulator* ptdpl = recevive_info->ptdpl;
			if(ptdpl != 0)
				delete ptdpl;

			free(recevive_info);

		}

		map_ptdpl->erase(recevive_it);

	}

	map<int,CLIENT_POST_INFO*>::iterator post_it;
	post_it = map_ptifo->find(clientfd);

	if(post_it != map_ptifo->end())
	{
		CLIENT_POST_INFO* post_info = post_it->second;
		if(post_info != 0)
		{
			if(m_sock == 0)
				m_sock = post_info->m_sock;
			

			CLBuffer* buffer = post_info->buffer;
			if(buffer != 0)
				delete buffer;

			free(post_info);
		}

		map_ptifo->erase(post_it);

	}

	 m_epoll->Register_ctl(EPOLL_CTL_DEL,clientfd,clientfd,0,EPOLLIN | EPOLLOUT); 

	if(m_sock != 0)
	{
		if( -1 == m_sock->CloseSocket())
		{
			throw "In CLCAServerByEpoll::FreeClientFd(),m_sock->CloseSocket() close socket error";
		}
		
		delete m_sock;
		return ;
	}

	if(-1 == close(clientfd))
	{
		throw "In CLCAServerByEpoll::FreeClientFd(),m_sock->CloseSocket() close socket error";
		return;
	}

}

void CLCAServerByEpoll::CheckForClientFd()
{
	map<int,CLIENT_RECEVIVER_INFO*>::iterator rev_it;
	map<int,CLIENT_POST_INFO*>::iterator post_it;

	CLIENT_POST_INFO* post_info = 0;
	CLIENT_RECEVIVER_INFO* rev_info = 0;

	int fd = -1;

	for(rev_it = map_ptdpl->begin(); rev_it != map_ptdpl->end() ; rev_it ++)
	{
		fd = rev_it->first;
		post_it = map_ptifo->find(fd);
		if(post_it == map_ptifo->end())
		{
			FreeClientFd(fd);
			continue;
		}

		rev_info = rev_it->second;
		post_info = post_it->second;



		if(rev_info->Logic_TimeStamp > 0)
			rev_info->Logic_TimeStamp --;
		else
			rev_info->IsDelete = true;



		if(rev_info->IsDelete && post_info->IsDelete)
		{
					FreeClientFd(fd);
					continue;
		}

	}


}