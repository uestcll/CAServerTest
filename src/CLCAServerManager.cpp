#include "CLCAServerManager.h"



CLCAServerManager::CLCAServerManager()
{
	server = new CLCAServerByEpoll(0,PORT);
	IsDeleteServer = true;

}

CLCAServerManager::CLCAServerManager(CLCAServer* ser)
{
	server = ser;
	IsDeleteServer = false;
}

CLCAServerManager::~CLCAServerManager()
{
	if(IsDeleteServer)
		delete server;

}

void CLCAServerManager::Initialize()
{
	server->Initialize();
	RegisterSerializer(PK_FORSGET,new CLCAGETPKMsgSerializer);
	RegisterSerializer(PK_FORMGET,new CLCAGETPKMutiMsgSerializer);

	RegisterDeSerializer(PK_FORSGET,new CLCAGETPKMsgDeSerializer);
	RegisterDeSerializer(PK_FORMGET,new CLCAGETPKMsgDeSerializer);

	RegisterHandler(PK_FORSGET,this->HandlerForGETPKMsg);
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
		return;

	uint32_t MsgType;
	list<CLCAClientContext*> ::iterator it;
	map<uint32_t msgID,CLCADeSerializer*> ::iterator DeSer_it;
	map<uint32_t msgID,Handler> ::iterator Handler_it;
	for(it = client_list->begin();it != client_list->end();it++)
	{
		MsgType = it->MsgType;
		DeSer_it = map_DeSer.find(MsgType);
		if(DeSer_it == map_DeSer.end())
			continue;
		CLCAMessage* msg = 0;

		msg = DeSer_it->second->DeSerializer(it->buf);

		Handler_it = map_Handler.find(MsgType);
		if(Handler_it == map_Handler.end())
			return;

		Handler_it->second(msg,(CLCAClientContext*)it);

	}


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

void CLCAServerManager::HandlerForGETPKMsg(CLCAMessage* msg,void* pContext)
{
	if(msg->m_MsgID != PK_FORSGET)
		return;

	CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>msg;
	if(message == 0)
		return;


	server->writeData();
}