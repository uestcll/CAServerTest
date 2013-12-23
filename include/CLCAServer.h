#ifndef CLCASERVER_H
#define CLCASERVER_H


#include <vector>
#include <map>
#include <stdint.h>
#include <sys/uio.h>
#include <sys/socket.h>

class CLSocket;
class CLCAMessage;
class CLCASerializer;
class CLCADeSerializer;
class CLBuffer;
class CLDataReceviver;
class CLProtocolDecapsulator;

struct CLIENT_RECEVIVER_INFO
{
	CLSocket* m_sock;
	CLProtocolDecapsulator* ptdpl;
	int TimeStamp;
	int Logic_TimeStamp;
	bool IsDelete;

	
};

struct CLIENT_POST_INFO
{
	CLSocket* m_sock;
	CLBuffer* buffer;
	bool IsDelete;

};

struct CLIENT_CLBUFFER_INFO
{
	std::vector<CLBuffer*>* buffer;
	int sock;
};

struct CLIENT_ONE_MSG_INFO //这里的One Msg是指 客户端一条报文中包含的消息（可能含有多条消息）
{
	std::vector<CLCAMessage*>* Msg;
	int sock;
	uint32_t MsgType;
};

struct CLIENT_MSG_INFO
{
	std::vector<CLIENT_ONE_MSG_INFO*>* Msg;
};
class CLCAServer
{
public:
	CLCAServer(const uint8_t* IP,const uint16_t Port,const int IPType = AF_INET,const int streamType = SOCK_STREAM);
	virtual ~CLCAServer();


	virtual int RegisterSerializer(uint32_t MsgId,CLCASerializer* ser);
	virtual int RegisterDeSerializer(uint32_t MsgId,CLCADeSerializer* deser);
	virtual void Initialize(CLDataReceviver* drecv) = 0;
	virtual int Accept() = 0;
	virtual CLIENT_MSG_INFO* getData() = 0;
	virtual int writeData(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId) = 0;
	virtual void start() = 0;
	virtual void run() = 0;

protected:
	CLSocket * m_sock;
	CLDataReceviver* m_drecv;
	std::map<int,CLIENT_RECEVIVER_INFO*>* map_ptdpl;
	std::map<int,CLIENT_POST_INFO*>* map_ptifo;
	std::map<uint32_t ,CLCADeSerializer*>* map_DeSer;
	std::map<uint32_t ,CLCASerializer*>* map_Ser;

};
#endif