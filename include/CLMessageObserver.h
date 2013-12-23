#ifndef CLMESSAGEOBSERVER_H
#define CLMESSAGEOBSERVER_H

#include <stdint.h>
#include <vector>

class CLCAServerManager;
class CLCAMessage;

class CLMessageObserver
{
public:
	CLMessageObserver();
	virtual ~CLMessageObserver();

	virtual void Initialize(CLCAServerManager* manager,void* pContext) = 0;
	virtual void WriteNetMsg(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId = 0);
};
#endif