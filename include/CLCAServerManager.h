#ifndef CLCASERVERMANAGER_H
#define CLCASERVERMANAGER_H

#include "CLCAServer.h"
#include "CLCASerializer.h"
#include "CLCADeSerializer.h"
#include "CLCAMessage.h"
#include "CLMessageObserver.h"
#include <list>
#include <map>
#define  PORT 19999

class CLMessageObserver;

typedef void (*Handler)(void*);
class CLCAServerManager
{

public:
	CLCAServerManager(CLCAServer* ser,CLMessageObserver* msgObserver);
	CLCAServerManager(CLMessageObserver* msgObserver);
	virtual ~CLCAServerManager();

	virtual void Initialize();
	virtual void RunLoop();
    
	virtual int RegisterSerializer(uint32_t msgID,CLCASerializer* ser);
	virtual int RegisterDeSerializer(uint32_t msgID,CLCADeSerializer* Deser);
	virtual int RegisterHandler(uint32_t msgID,Handler handler);

protected:
	virtual void Dispatch(void* pContext);



protected:
	CLCAServer* server;
	bool IsDeleteServer;
	std::map<uint32_t ,CLCADeSerializer*> map_DeSer;
	std::map<uint32_t ,CLCASerializer*> map_Ser;
	std::map<uint32_t ,Handler> map_Handler;
	CLMessageObserver* m_pMsgObserver;
};
#endif