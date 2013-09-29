#ifndef CLMESSAGEOBSERVER_H
#define CLMESSAGEOBSERVER_H

#include "CLCAServerManager.h"
#include "CLCAMessage.h"
#include "CLCAClientContext.h"

class CLMessageObserver;

struct HandlerContext
{
	CLCAMessage* msg;
	CLMessageObserver* MsgObserver;
	CLCAClientContext* context;
	void* pContext;
};

class CLMessageObserver
{
public:
	CLMessageObserver();
	virtual ~CLMessageObserver();

	virtual void Initialize(CLCAServerManager* manager,void* pContext) = 0;
};
#endif