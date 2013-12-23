#include "CLCAClientPostNetMsgBySocket.h"
#include "CLDataReceviverBySocket.h"
#include "CLCAAddress.h"
#include "CLSocket.h"
#include "CLDataReceviver.h"
#include "CLEpoll.h"
#include "CLCASerializer.h"
#include "CLCADeSerializer.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMutiMsgSerializer.h"
#include "CLCAREGETPKMsgDeSerializer.h"
#include "CLCAREGETPKMutiMsgDeSerializer.h"
#include "CLProtocolDecapsulator.h"
#include "CLCAServerMsgProtocolDecapsulator.h"
#include "CLCAMessage.h"

#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>



using namespace std;

CLCAClientPostNetMsgBySocket::CLCAClientPostNetMsgBySocket(uint8_t IPType,uint8_t* IP,uint16_t Port):CLCAPostNetMsgTool(IPType,IP,Port)
{
	map_ser = new map<int,CLCASerializer*>;
	map_deser = new map<int,CLCADeSerializer*>;
	m_recv = new CLDataReceviverBySocket;
	m_pdl = new CLCAServerMsgProtocolDecapsulator;

	Initialize();
}

CLCAClientPostNetMsgBySocket::~CLCAClientPostNetMsgBySocket()
{
	map<int,CLCASerializer*>::iterator ser_it;
	CLCASerializer* ser = 0;

	for(ser_it = map_ser->begin() ; ser_it != map_ser->end(); ser_it ++)
	{
		ser = ser_it->second;
		delete ser;
	}

	map<int,CLCADeSerializer*>::iterator deser_it;
	CLCADeSerializer* deser = 0;

	for(deser_it = map_deser->begin() ; deser_it != map_deser->end() ; deser_it ++)
	{
		deser =deser_it->second;
		delete deser;
	}

	delete map_ser;
	delete map_deser;
	delete m_recv;
	delete m_pdl;
	delete m_sock;
}

void CLCAClientPostNetMsgBySocket::PostNetMessage(vector<CLCAMessage*>* vec,int MsgType)
{
	if(vec == 0 || vec->size() == 0)
		return;

	if(MsgType == 0)
		return;

	uint8_t* buf = 0;
	uint32_t buf_size = 0;

	map<int,CLCASerializer*>::iterator it;
	it = map_ser->find(MsgType);

	if(it == map_ser->end())
		return;

	CLCASerializer* ser = it->second;
	buf = ser->Serialize(0,vec,0,&buf_size);

	if(buf == 0 || buf_size == 0)
		return;

	int ret = writeNetMsg(buf,buf_size);

	if(ret == -1)
	{
		while(true)
		{
			if(Connect() == 0)
				break;
		}
	}

	delete buf;

}

void CLCAClientPostNetMsgBySocket::PostSingleNetMessage(CLCAMessage* msg)
{
	if(msg == 0)
		return;

	uint8_t* buf = 0;
	uint32_t buf_size = 0;

	map<int,CLCASerializer*>::iterator it;
	it = map_ser->find(msg->m_MsgID);

	if(it == map_ser->end())
		return;

	CLCASerializer* ser = it->second;
	buf = ser->Serialize(msg,0,0,&buf_size);

	if(buf == 0 || buf_size == 0)
		return;

	int ret = writeNetMsg(buf,buf_size);

	if(ret == -1)
	{
		while(true)
		{
			if(Connect() == 0)
				break;
		}
	}

	delete buf;
}

void CLCAClientPostNetMsgBySocket::Initialize()
{
	m_epoll = 0;
	m_sock = new CLSocket(m_IP,m_Port);
//	m_sock->setNonBlock();
	
	RegisterSerializer(PK_FORSGET,new CLCAGETPKMsgSerializer);
	RegisterSerializer(PK_FORMGET,new CLCAGETPKMutiMsgSerializer);

	RegisterDeSerializer(PK_FORRESGET,new CLCAREGETPKMsgDeSerializer);
	RegisterDeSerializer(PK_FORREMGET,new CLCAREGETPKMutiMsgDeSerializer);
	Connect();
}

int CLCAClientPostNetMsgBySocket::Connect()
{
	return m_sock->ConnectSocket();
}

int CLCAClientPostNetMsgBySocket::RegisterSerializer(int type,CLCASerializer* ser)
{
	if(map_ser->find(type) != map_ser->end())
		return -1;
	
	map_ser->insert(pair<int,CLCASerializer*>(type,ser));

	return 0;
}

int CLCAClientPostNetMsgBySocket::RegisterDeSerializer(int type,CLCADeSerializer* deser)
{
	if(map_deser->find(type) != map_deser->end())
		return -1;

	 map_deser->insert(pair<int,CLCADeSerializer*>(type,deser));

	 return 0;
}

int CLCAClientPostNetMsgBySocket::writeNetMsg(uint8_t* msg,uint32_t length)
{

	uint8_t* buf = msg;
	uint32_t len = length;
	uint32_t HasWriteLen = 0;
	int writeLen = 0;
	while(HasWriteLen<length)
	{
	
		writeLen = m_sock->WriteSocket(buf+HasWriteLen,len);
		
		if(writeLen == -1 )
		{
			if(errno != EAGAIN && errno != EINTR)
				return -1;
			else 
				continue;
		}


		HasWriteLen += writeLen;
		len -= writeLen;
	}

	return 0;

}

void CLCAClientPostNetMsgBySocket::InsertIntoVec(std::vector<CLCAMessage*>* dz,std::vector<CLCAMessage*>* src)
{
	if(src == 0 || src->size() == 0 || dz == 0)
		return;

	for(int i = 0; i< src->size() ;i++)
		dz->push_back(src->at(i));

	delete src;

}

void CLCAClientPostNetMsgBySocket::startEpollForRead()
{
	m_epoll = new CLEpoll(100);
	m_epoll->Register_ctl(EPOLL_CTL_ADD,m_sock->getSock(),m_sock->getSock(),0,EPOLLIN);
}

void* CLCAClientPostNetMsgBySocket::ReadFromNet()
{
	startEpollForRead();

	int fd = -1;
	uint8_t* buf = 0;
	uint32_t buf_size = 0;
	uint32_t Left_size = 0;
	vector<CLBuffer*>* buffer_vec = new vector<CLBuffer*>;
//	bool AllBreak = false;
	vector<CLCAMessage*>* msg_vec = new vector<CLCAMessage*>;
	vector<CLCAMessage*>* temp_vec = 0;

	while(true)
	{
		int fds = m_epoll->EpollWait();
		if(fds != 1)
			continue;

		m_epoll->getEventFd(0,&fd);
		if(fd == m_sock->getSock())
		{
			Left_size = m_pdl->getLeftSize();
			m_recv->setContext(m_sock);
			buf = (uint8_t*)m_recv->getData(Left_size);
			buf_size = m_recv->getDataSize();

			if(buf_size == 0 )
			{
				if(m_recv->geterrno() < 0)
					break;

				if(m_recv->geterrno() != EAGAIN && m_recv->geterrno() != EINTR)
				{
					break;
				}

				continue;
			}

			m_pdl->addCharToDecapsulate(buf,buf_size);
			m_pdl->ProtocolDecapsulate();
				
			if(m_pdl->getLeftSize() == 0)
			{
				break;
			}
			
			
		}

	}

	buffer_vec = m_pdl->getDecapsulatorMsgChar();
	if(buffer_vec == 0)
		return 0;

	if(buffer_vec->size() == 0)
		return 0;

	CLBuffer* buffer = 0;
	uint32_t type = 0;
	bool IsDelete = false;
	map<int,CLCADeSerializer*>::iterator it;

	for(int i = 0; i< buffer_vec->size() ; i++)
	{
		buffer = buffer_vec->at(i);
		buf = buffer->getBuf(0,4,&IsDelete);

		type = ntohl(*(uint32_t*)buf);

		it = map_deser->find(type);
		if(it == map_deser->end())
		{
			cout<<"map_deser find error"<<endl;
			continue;
		}

		temp_vec = it->second->DeSerializer(buffer,8);
		InsertIntoVec(msg_vec,temp_vec);

	}

	return msg_vec;

}