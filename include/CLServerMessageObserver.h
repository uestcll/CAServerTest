#ifndef CLSERVERMESSAGEOBSERVER_H
#define CLSERVERMESSAGEOBSERVER_H

#include "CLMessageObserver.h"

class CLServerMessageObserver : public CLMessageObserver
{
public:
	CLServerMessageObserver();
	virtual ~CLServerMessageObserver();

	virtual void Initialize(CLCAServerManager* Manager,void* pContext);

	void HandlerForSGETPKMsg(CLCAMessage* msg,void* pContext);
	void HandlerForMGETPKMsg(CLCAMessage* msg,void* pContext);
private:

	CLCAServerManager* manager;

};
#endif