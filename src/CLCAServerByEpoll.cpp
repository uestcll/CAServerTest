#include "CLCAServerByEpoll.h"
#include "CLEpoll.h"
#include "CLCAServerMsgProtocolDecapsulator.h"
#include "CLDataReceviverBySocket.h"
#include "CLLogger.h"
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

void CLCAServerByEpoll::Initialize(CLDataReceviver* drecv)
{
	if( -1 == m_sock->BindSocket())
		throw "In CLCAServerByEpoll::Initialize(),BindSocket error";

	if( -1 == m_sock->ListenSocket(50))
		throw "In CLCAServerByEpoll::Initialize(),ListenSocket error";

	if( -1 == m_sock->setNonBlock())
		throw "In CLCAServerByEpoll::Initialize(),setNonBlock error";

	m_epoll = new CLEpoll();
	m_epoll->Register_ctl(EPOLL_CTL_ADD,m_sock->getSock(),m_sock->getSock(),0);
	
	m_drecv = drecv;
	//m_drecv = new CLDataReceviverBySocket;
	
}

void CLCAServerByEpoll::start()
{
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
			CLLogger::WriteLog("In CLCAServerByEpoll::getData(),getEventFd error",0);
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
						CLLogger::WriteLog("In CLCAServerByEpoll::run(),AcceptSocket error",0);
					
					break;
				}

				if(HandleAccept(clientFd) == -1)
				{
					CLLogger::WriteLog("In CLCAServerByEpoll::run(),HandleAccept error",0)
					continue;
				}

			}
		
		}

		else if(m_epoll->getEvents()[i].events & EPOLLIN)
		{
			cout<<"GetData From client"<<endl;
			
			ret = getClientData(fd);
			if(ret == 0)
				continue;

			if(ret->buffer->size() == 0)
			{
				delete ret->buffer;
				free(ret);
				continue;
			}

			client_Msg->push_back(ret);

		}

		else if(m_epoll->getEvents()[i].events & EPOLLOUT)
		{
			map<int,CLIENT_POST_INFO*>::iterator it;
			it = map_ptifo->find(fd);

			if(it == map_ptifo->end())
			{
				CLLogger::WriteLog("In CLCAServerByEpoll::run(),fd error",0);
				m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLOUT | EPOLLET);
				continue;
			}

			CLIENT_POST_INFO* info = it->second;
			if(info == 0)
			{
				CLLogger::WriteLog("In CLCAServerByEpoll::run(),CLIENT_POST_INFO null",0);
				m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLOUT );
				continue;
			}

			uint32_t currentIovecIndex = info->buffer->getIovecIndex();

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
					CLLogger::WriteLog("In CLCAServerByEpoll::run(),writevSocket error",0);
					FreeClientFd(fd);
					continue;
				}
			}

			if(len == 0)
			{
				continue;
			}

			if(len == info->buffer->getBufferToIovecBufferSize())
			{
				delete info->buffer;
				info->buffer = 0;
				m_epoll->Register_ctl(EPOLL_CTL_DEL,fd,fd,0,EPOLLOUT );

				continue;
			}

			if(len < info->buffer->getBufferToIovecBufferSize())
			{
				info->buffer->setIovecIndex(currentIovecIndex + len);
				continue;
			}

		}

		else
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
			MsgId = *(uint32_t*)buf;
			
			if(IsDeleted)
				delete buf;
			  
			DeserIt = map_DeSer->find(MsgId); 
			if(DeserIt == map_DeSer->end())
			{
				CLLogger::WriteLog("In CLCAServerByEpoll::getData(),find CLCASerializer failed",0);
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
	CLSocket* sock = new CLSocket(clientFd);

	if( -1 == sock->setNonBlock())
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::HandleAccept(),setNonBlock error",0);
	}

	if( -1 == m_epoll->Register_ctl(EPOLL_CTL_ADD,sock->getSock(),sock->getSock(),0,EPOLLIN | EPOLLET))
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::HandleAccept(),Register_ctl error",0);
		delete sock;
		close(clientFd);
		return -1;
	}

	if(map_ptdpl->find(clientfd) != map_ptdpl->end())
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::HandleAccept(),map<int,CLIENT_RECEVIVER_INFO*> find fd error",0);
		delete sock;
		return -1;
	}

	CLProtocolDecapsulator* ptdpl = new CLCAServerMsgProtocolDecapsulator;
	CLIENT_RECEVIVER_INFO* info = (CLIENT_RECEVIVER_INFO*)malloc(sizeof(struct CLIENT_RECEVIVER_INFO));
	info->m_sock = sock;
	info->ptdpl = ptdpl;
	info->IsDelete = false;

	map_ptdpl->insert(pair<int,CLIENT_RECEVIVER_INFO*>(clientFd,info));

	if(map_ptifo->find(clientFd) != map_ptifo->end())
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::HandleAccept(),map<int,CLIENT_POST_INFO*> find fd error",0);
		delete sock;
		return -1;
	}

	CLIENT_POST_INFO* post_info = (CLIENT_POST_INFO*)malloc(sizeof(CLIENT_POST_INFO));
	post_info->buffer = 0;
	post_info->m_sock = sock;
	post_info->IsDelete = true;

	map_ptifo->insert(pair<int,CLIENT_POST_INFO*>(clientFd,post_info));

	return 0;
}

CLIENT_CLBUFFER_INFO* CLCAServerByEpoll::getClientData(int clientfd)
{
	if(map_ptdpl->find(clientfd) == map_ptdpl->end())
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::getClientData(),map<int,CLProtocolDecapuslator> find clientfd failed",0);
		return 0;
	}

	CLIENT_RECEVIVER_INFO* info = map_ptdpl->find(clientfd)->second;
	info->IsDelete = false;
	CLSocket* sock = info->m_sock;
	CLProtocolDecapsulator* ptdpl = info->ptdpl;
	m_drecv->setContext(sock);

	uint32_t leftSize = ptdpl->getLeftSize();
	while(true)
	{

		uint8_t* buf = m_drecv->getData(leftSize);
		uint32_t len = m_drecv->getDataSize();

		if(len == 0)
		{
			if(buf != 0)
				delete buf;
		/////
			if( m_drecv->geterrno() > 0 && (m_drecv ->geterrno() != EAGAIN || m_drecv->geterrno() != EINTR) )
			{
				CLLogger::WriteLog("In CLCAServerByEpoll::getClientData(), m_drecv ->getdata() error",0);
				FreeClientFd(fd);
			}
			//read调用返回0
			else if(m_drecv->geterrno() <= 0)
			{
				info->IsDelete  = true;
			}


			break;
		}

		ptdpl->addCharToDecapsulate(buf,len);
		ptdpl->ProtocolDecapsulate();
		if(ptdpl->getLeftSize() == 0)
		{
			// 解析完毕的处理 。。。。
		}

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
		return 0;
	}
	

	return CLBuffer_info;

}

//该函数实现 将msg序列化 ，并通知epoll写
int CLCAServerByEpoll::writeData(vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId)
{
	if(msg_vec == 0 )
	{
		CLLogger::WriteLog("In CLCAServerByEpoll::writeData(),msg_vec null",0);
		return -1;
	}

	if(msg_vec->size() == 0)
	{
		delete msg_vec;
		CLLogger::WriteLog("In CLCAServerByEpoll::writeData(),msg_vec size is 0",0);
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
		CLLogger::WriteLog("In CLCAServerByEpoll::writeData(),sock not find",0);
		return -1;
	}


	CLIENT_POST_INFO* post_info = 0;
	CLBuffer* buffer = 0;
	CLCAMessage* msg = 0;

	post_info = (*post_info_it);
	if(post_info->buffer != 0)
		buffer = post_info->buffer;


	if(IsMutiMsg)
		MsgType = MsgId;
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
		CLLogger::WriteLog("In CLCAServerByEpoll::writeData(),CLCASerializer not find",0);
		
		//如果是多条消息序列化一条报文，没有找到序列化类 则 删除全部 。
		if(IsMutiMsg)  
		{
			for( msg_it = msg_vec->begin(); msg_it != msg_vec->end() ;msg_it ++)
			{
				if( (*msg_it) != 0)
					delete *msg_it;
			}

			delete msg_vec;
			return -1;
		}
	   //否则直到找到一条有序列化类的消息
		else
		{
			do
			{ 
				if(msg != 0)
					delete msg;

				msg_it ++ ;
				msg = (*msg_it);

				if(msg != 0)
					MsgType = msg->m_MsgID;
				else
					continue;

				Ser_It = map_Ser->find(MsgType);
				if(Ser_It != map_Ser->end())
					break;
			
			}while(msg_it != msg_vec->end())
		}

		if(msg_it == msg_vec->end())
		{
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
		ser->Serialize(msg);
		ser->SerializeHead(msg->m_MsgID,1);
		
		buf = ser->getSerializeChar();
		buf_size = ser->getFullLength();

		buffer->addBuf(buf,buf_size,CANDELETE);

	}
	else
	{

		uint32_t sum = 0;
		uint32_t SingleMsgType = (*msg_it)->m_MsgID ;

		if(IsMutiMsg)
		{
			while(msg_it != msg_vec->end())
			{
				msg = (*msg_it);
				if(msg->m_MsgID != SingleMsgType)
				{
					if(sum == 0)
						break; //这里sum 不可能为 0
				
					ser->SerializeHead(MsgType,sum);

					buf = ser->getSerializeChar();
					buf_size = ser->getFullLength();
					buffer->addBuf(buf,buf_size,CANDELETE);

					sum = 0;
						break;

				}
					ser->Serialize(msg);
					sum++;
					msg_it++;
			}
		}
		// sum == 0 表示并非全部都是同一类型msg，需要进行逐个序列化
		if(sum != 0)
		{
			ser->SerializeHead(MsgType,sum);
			buf = ser->getSerializeChar();
			buf_size = ser->getFullLength();
			buffer->addBuf(buf,buf_size,CANDELETE);
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
				
				if(Ser_It == map_Ser->end();)
				{
					CLLogger::WriteLog("In CLCAServerByEpoll::writeData(),CLCASerializer not find",0);
					delete msg;
					continue;
				}

				ser = Ser_It->second;

				ser->Serialize(msg);
				ser->SerializeHead(MsgType,1);
				buf = ser->getSerializeChar();
				buf_size = ser->getFullLength();
				buffer->addBuf(buf,buf_size,CANDELETE);


			}
		}

	}

	m_epoll->Register_ctl(EPOLL_CTL_ADD,sock,sock,0,EPOLLOUT | EPOLLET);
	return 0;
}

//释放clientfd相关资源
void CLCAServerByEpoll::FreeClientFd(int clientfd)
{

	CLSocket* m_sock = 0;
	map<int,CLIENT_RECEVIVER_INFO*>::iterator recevive_it;
	
	recevive_it = map_ptdpl->find();
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