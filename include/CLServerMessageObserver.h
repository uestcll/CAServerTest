#ifndef CLSERVERMESSAGEOBSERVER_H
#define CLSERVERMESSAGEOBSERVER_H

#include "CLMessageObserver.h"

class CLServerMessageObserver : public CLMessageObserver
{
public:
	CLServerMessageObserver();
	virtual ~CLServerMessageObserver();

	virtual void Initialize(CLCAServerManager* Manager,void* pContext);
	virtual void WriteNetMsg(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgType = 0);
	static void HandlerForSGETPKMsg(void* pContext);
	static void HandlerForMGETPKMsg(void* pContext);
	static void HandlerForSUPDATEMsg(void* pContext);
	static void HandlerForMUPDATEMsg(void* pContext);
private:

	CLCAServerManager* manager;

};
#endif