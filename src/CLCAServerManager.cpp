#include "CLCAServerManager.h"
#include "CLCAServerByEpoll.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMutiMsgSerializer.h"
#include "CLCAGETPKMsgDeSerializer.h"
#include "CLCAGETPKMutiMsgDeSerializer.h"
#include "CLCAGETPKMessage.h"
#include <stdlib.h>



using namespace std;


CLCAServerManager::CLCAServerManager(CLMessageObserver* msgObserver)
{
	server = new CLCAServerByEpoll(0,PORT);
	IsDeleteServer = true;
	m_pMsgObserver = msgObserver;

}

CLCAServerManager::CLCAServerManager(CLCAServer* ser,CLMessageObserver* msgObserver)
{
	server = ser;
	IsDeleteServer = false;
	m_pMsgObserver = msgObserver;
}

CLCAServerManager::~CLCAServerManager()
{
	if(IsDeleteServer)
		delete server;

}

void CLCAServerManager::Initialize()
{

	server->Initialize();
	m_pMsgObserver->Initialize(this,0);
	RegisterSerializer(PK_FORSGET,new CLCAGETPKMsgSerializer);
	RegisterSerializer(PK_FORMGET,new CLCAGETPKMutiMsgSerializer);

	RegisterDeSerializer(PK_FORSGET,new CLCAGETPKMsgDeSerializer);
	RegisterDeSerializer(PK_FORMGET,new CLCAGETPKMutiMsgDeSerializer);

}

void CLCAServerManager::RunLoop()
{
	list<CLCAClientContext*>* client_list;
	while(true)
	{
		server->start();
		client_list = server->getData();
		Dispatch(client_list);

	}
}

void CLCAServerManager::Dispatch(void* pContext)
{
	if(pContext == 0)
		return;

	list<CLCAClientContext*>* client_list = (list<CLCAClientContext*>*)pContext;
	if(client_list->empty())
	{
		delete client_list;
		return;
	}
	uint32_t MsgType;
	list<CLCAClientContext*> ::iterator it;
	map<uint32_t,CLCADeSerializer*> ::iterator DeSer_it;
	map<uint32_t,Handler> ::iterator Handler_it;
	for(it = client_list->begin();it != client_list->end();it++)
	{
		MsgType = (*it)->MsgType;
		DeSer_it = map_DeSer.find(MsgType);
		if(DeSer_it == map_DeSer.end())
			continue;
		CLCAMessage* msg = 0;

		msg = DeSer_it->second->DeSerializer((*it)->buf);
#ifdef GETSPKMsg_Debug
		char* msgName = new char[20];
		memset(msgName,0,20);
		sprintf(msgName,"%s%d",PostMsgName,msg_id);
		CLCAGETPKMessage* testMsg = (CLCAGETPKMessage*)msg;
		EXPECT_FALSE(msg == 0);
		EXPECT_EQ(testMsg->LengthOfName,strlen(msgName)+1);
		EXPECT_FALSE(strcmp(testMsg->Name,msgName));
		EXPECT_EQ(testMsg->PKType,1);
		EXPECT_EQ(testMsg->EchoID,msg_id);
		EXPECT_EQ(testMsg->m_MsgID,PK_FORSGET);
		delete msgName;
		msg_id++;
#endif
		Handler_it = map_Handler.find(MsgType);
		if(Handler_it == map_Handler.end())
			continue;

		HandlerContext* context = (HandlerContext*)malloc(sizeof(struct HandlerContext));
		context->context = *it;
		context->msg = msg;
		context->MsgObserver = this->m_pMsgObserver;
		context->pContext = 0;
		Handler_it->second(context);
		free(context);
		if((*it)->MsgNum > 1)
			delete []msg;
		else
			delete msg;
	
		delete (*it);

	}

	client_list->clear();
	delete client_list;



}

int CLCAServerManager::RegisterSerializer(uint32_t msgID,CLCASerializer* ser)
{
	map<uint32_t,CLCASerializer*> ::iterator it;
	it = map_Ser.find(msgID);
	if (it != map_Ser.end())
		return -1;
	
	map_Ser[msgID] = ser;

	return 0;
}

int CLCAServerManager::RegisterDeSerializer(uint32_t msgID,CLCADeSerializer* Deser)
{
	map<uint32_t,CLCADeSerializer*> ::iterator it;
	it = map_DeSer.find(msgID);
	if (it != map_DeSer.end())
		return -1;

	map_DeSer[msgID] = Deser;

	return 0;
}

int CLCAServerManager::RegisterHandler(uint32_t msgID,Handler handler)
{
	map<uint32_t,Handler> ::iterator it;
	it = map_Handler.find(msgID);
	if (it != map_Handler.end())
		return -1;

	map_Handler[msgID] = handler;

	return 0;
}

