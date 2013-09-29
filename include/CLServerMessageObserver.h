#ifndef CLSERVERMESSAGEOBSERVER_H
#define CLSERVERMESSAGEOBSERVER_H

#include "CLMessageObserver.h"

class CLServerMessageObserver : public CLMessageObserver
{
public:
	CLServerMessageObserver();
	virtual ~CLServerMessageObserver();

	virtual void Initialize(CLCAServerManager* Manager,void* pContext);

	static void HandlerForSGETPKMsg(void* pContext);
	static void HandlerForMGETPKMsg(void* pContext);
private:

	CLCAServerManager* manager;

};
#endif