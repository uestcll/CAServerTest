#include "CLServerMessageObserver.h"
#include "CLCAServerManager.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCAClientContext.h"
#include "CLCAREGETPKMessage.h"
#include "CLCAREGETPKMsgSerializer.h"

CLServerMessageObserver::CLServerMessageObserver()
{

}

CLServerMessageObserver::~CLServerMessageObserver()
{

}

void CLServerMessageObserver::Initialize(CLCAServerManager* Manager,void* pContext)
{
	manager = Manager;
	manager->RegisterHandler(PK_FORSGET,(Handler)(&CLServerMessageObserver::HandlerForSGETPKMsg));
	manager->RegisterHandler(PK_FORMGET,(Handler)(&CLServerMessageObserver::HandlerForMGETPKMsg));
}

void CLServerMessageObserver::HandlerForSGETPKMsg(void* pContext)
{
	HandlerContext* context = (HandlerContext*)pContext;
	CLCAMessage* msg = context->msg;
	if(msg->m_MsgID != PK_FORSGET)
		return;

	CLServerMessageObserver* observer = (CLServerMessageObserver*)context->MsgObserver;
	CLCAClientContext* clientcon = context->context;
	CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>(msg);
	if(message == 0)
		return;

	CLCAREGETPKMessage* remsg = new CLCAREGETPKMessage(0,1,0,0,message->EchoID);
	CLCASerializer* ser = new CLCAREGETPKMsgSerializer;
	ser->Serialize(remsg);
	ser->SerializeHead(PK_FORRESGET,1);
	uint8_t* serchar = ser->getSerializeChar();
	clientcon->m_sock->WriteSocket(serchar,ser->getFullLength());
	delete remsg;
	delete ser;
	delete serchar;


}

void CLServerMessageObserver::HandlerForMGETPKMsg(void* pContext)
{

}