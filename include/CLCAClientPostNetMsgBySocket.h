#ifndef CLCACLIENTPOSTNETMSGBYSOCKET_H
#define CLCACLIENTPOSTNETMSGBYSOCKET_H

#include "CLCAPostNetMsgTool.h"

#include <stdint.h>
#include <vector>
#include <map>

class CLCAAddress;
class CLSocket;
class CLDataReceviver;
class CLEpoll;
class CLCAMessage;
class CLCASerializer;
class CLCADeSerializer;
class CLProtocolDecapsulator;

class CLCAClientPostNetMsgBySocket: public CLCAPostNetMsgTool
{
public:

	CLCAClientPostNetMsgBySocket(uint8_t IPType,uint8_t* IP,uint16_t Port);
	virtual ~CLCAClientPostNetMsgBySocket();

	virtual void PostNetMessage(std::vector<CLCAMessage*>* vec , int type);
	virtual void PostSingleNetMessage(CLCAMessage* msg);
	virtual void* ReadFromNet();

	void startEpollForRead();
	
private:
	void Initialize();
	int Connect();
	int writeNetMsg(uint8_t* msg,uint32_t length);
	int RegisterSerializer(int type,CLCASerializer* ser);
	int RegisterDeSerializer(int type,CLCADeSerializer* deser);

	void InsertIntoVec(std::vector<CLCAMessage*>* dz,std::vector<CLCAMessage*>* src);

private:

	CLCAAddress* m_addr;
	CLEpoll* m_epoll;
	CLSocket * m_sock;
	CLDataReceviver* m_recv;
	std::map<int,CLCASerializer*>* map_ser;
	std::map<int,CLCADeSerializer*>* map_deser;
	CLProtocolDecapsulator* m_pdl;
};
#endif