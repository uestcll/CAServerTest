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

	void* Msg; 
	while(true)
	{
		server->start();
		server->run();
		Msg = server->getData();
		Dispatch(Msg);

	}
}

void CLCAServerManager::Dispatch(void* pContext)
{
	if(pContext == 0)
		return;

	CLIENT_MSG_INFO* Msg_Info = (CLIENT_MSG_INFO*)pContext;

	if(Msg_Info == 0)
		return;

	if(Msg_Info->Msg->size() == 0)
	{
		delete Msg_Info->Msg;
		free(Msg_Info);
		return ;
	}

	vector<CLIENT_ONE_MSG_INFO*>* One_Msg_Vec = Msg_Info->Msg;
	vector<CLIENT_ONE_MSG_INFO*>::iterator One_Msg_it;

	vector<CLCAMessage*>::iterator it;
	map<uint32_t,Handler> ::iterator Handler_it;
	uint32_t MsgType ;

	for(One_Msg_it = One_Msg_Vec->begin();One_Msg_it != One_Msg_Vec->end();it++)
	{
		MsgType = (*One_Msg_it)->MsgType;

		Handler_it = map_Handler.find(MsgType);
		if(Handler_it == map_Handler.end())
			continue;

		HandlerStruct* str = (HandlerStruct*)malloc(sizeof(HandlerStruct));
		str->observer = m_pMsgObserver;
		str->MsgData = (*One_Msg_it)->Msg;
		str->sock = (*One_Msg_it)->sock;;

		Handler_it->second(str);

		for(it = (*One_Msg_it)->Msg->begin(); it != (*One_Msg_it)->Msg->end();it++)
		{
			if((*it) != 0)
				delete *it;

		}

		delete (*One_Msg_it)->Msg;
		free(str);
		free(*One_Msg_it);
	}

	delete One_Msg_Vec;
	free(Msg_Info);



}

void CLCAServerManager::WriteMsg(std::vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId)
{
	server->writeData(msg_vec,sock,IsMutiMsg,MsgId);
}

int CLCAServerManager::RegisterSerializer(uint32_t msgID,CLCASerializer* ser)
{
	return server->RegisterSerializer(msgID,ser);
}

int CLCAServerManager::RegisterDeSerializer(uint32_t msgID,CLCADeSerializer* Deser)
{
	return server->RegisterDeSerializer(msgID,Deser);
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

