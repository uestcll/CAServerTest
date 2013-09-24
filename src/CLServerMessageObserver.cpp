#include "CLServerMessageObserver.h"
#include "CLCAServerManager.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCAClientContext.h"

CLServerMessageObserver::CLServerMessageObserver()
{

}

CLServerMessageObserver::~CLServerMessageObserver()
{

}

void CLServerMessageObserver::Initialize(CLCAServerManager* Manager,void* pContext)
{
	manager = Manager;
	manager->RegisterHandler(PK_FORSGET,(Handler)(&CLServerMessageObserver::HandlerForGETPKMsg));
}

void CLServerMessageObserver::HandlerForSGETPKMsg(CLCAMessage* msg,void* pContext)
{
	if(msg->m_MsgID != PK_FORSGET)
		return;

	CLCAClientContext* context = (CLCAClientContext*)pContext;
	CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>msg;
	if(message == 0)
		return;


	manager->server->writeData();

	delete context;

}

void CLServerMessageObserver::HandlerForMGETPKMsg(CLCAMessage* msg,void* pContext)
{

}