#ifndef CLMESSAGEOBSERVER_H
#define CLMESSAGEOBSERVER_H

class CLCAServerManager;

class CLMessageObserver
{
public:
	CLMessageObserver();
	virtual ~CLMessageObserver();

	virtual void Initialize(CLCAServerManager* manager,void* pContext) = 0;
};
#endif