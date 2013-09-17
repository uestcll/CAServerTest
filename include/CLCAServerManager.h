#ifndef CLCASERVERMANAGER_H
#define CLCASERVERMANAGER_H

#include "CLCAServer.h"
#include "CLCASerializer.h"
#include "CLCADeSerializer.h"
#include "CLCAMessage.h"
#include <list>
#define  PORT 19999

typedef void (*Handler)(CLCAMessage*,void* pContext);
class CLCAServerManager
{

public:
	CLCAServerManager(CLCAServer* ser);
	CLCAServerManager();
	virtual ~CLCAServerManager();

	virtual void Initialize();
	virtual void RunLoop();
    
	virtual int RegisterSerializer(uint32_t msgID,CLCASerializer* ser);
	virtual static static static static virtual int RegisterDeSerializer(uint32_t msgID,CLCADeSerializer* Deser) const const;
	virtual int RegisterHandler(uint32_t msgID,Handler handler);

private:
	
	virtual void Dispatch(void* pContext);

	void HandlerForGETPKMsg(CLCAMessage* msg,void* pContext);
protected:
	CLCAServer* server;
	bool IsDeleteServer;
	map<uint32_t msgID,CLCADeSerializer*> map_DeSer;
	map<uint32_t msgID,CLCASerializer*> map_Ser;
	map<uint32_t msgID,Handler> map_Handler;
};
#endif