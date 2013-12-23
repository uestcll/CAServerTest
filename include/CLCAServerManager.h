#ifndef CLCASERVERMANAGER_H
#define CLCASERVERMANAGER_H

#include <stdint.h>
#include <map>
#include <vector>

#define  PORT 8080


class CLMessageObserver;
class CLCAServer;
class CLCADeSerializer;
class CLCASerializer;
class CLCAMessage;

struct HandlerStruct
{
	int sock;
	void* MsgData;  
	CLMessageObserver* observer;
};


typedef void (*Handler)(void*);
class CLCAServerManager
{

public:
	CLCAServerManager(CLCAServer* ser,CLMessageObserver* msgObserver);
	CLCAServerManager(CLMessageObserver* msgObserver);
	virtual ~CLCAServerManager();

	virtual void Initialize();
	virtual void RunLoop();

	virtual int RegisterSerializer(uint32_t MsgId,CLCASerializer* ser);
	virtual int RegisterDeSerializer(uint32_t MsgId,CLCADeSerializer* deser);
	virtual int RegisterHandler(uint32_t msgID,Handler handler);

	virtual void WriteMsg(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId);
	

protected:
	virtual void Dispatch(void* pContext);



protected:
	CLCAServer* server;
	bool IsDeleteServer;
	std::map<uint32_t ,Handler> map_Handler;
	CLMessageObserver* m_pMsgObserver;
};
#endif