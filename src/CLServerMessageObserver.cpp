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

void CLServerMessageObserver::HandlerForSGETPKMsg(void* pContext)
{
	HandlerContext* context = (HandlerContext*)pContext;
	CLCAMessage* msg = context->msg;
	if(msg->m_MsgID != PK_FORSGET)
		return;

	CLServerMessageObserver* observer = (CLServerMessageObserver*)context->MsgObserver;
	CLCAClientContext* clientcon = context->context;
	CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>msg;
	if(message == 0)
		return;

	

}

void CLServerMessageObserver::HandlerForMGETPKMsg(void* pContext)
{

}