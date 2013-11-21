#include "CLServerMessageObserver.h"
#include "CLCAServerManager.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCAClientContext.h"
#include "CLCAREGETPKMessage.h"
#include "CLCAREGETPKMsgSerializer.h"
#include "CLSqlite.h"
#include "CLLogger.h"

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

void CLServerMessageObserver::WriteNetMsg(vector<CLCAMessage*>* msg_vec,int sock,bool IsMutiMsg,uint32_t MsgId)
{
	manager->WriteMsg(msg_vec,sock);
}

void CLServerMessageObserver::HandlerForSGETPKMsg(void* pContext)
{
	if(pContext == 0 )
		return;
	try
	{
		HandlerStruct* hs = (HandlerStruct*)pContext;
		CLMessageObserver* oberver = (CLMessageObserver*)hs->observer;
		vector<CLCAMessage*>* vec = (vector<CLCAMessage*>*)hs->MsgData;
		if(vec == 0 || vec->size() != 1 )
		{
			CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForSGETPKMsg(),vec size error",0);
			return ;
		}

		CLCAGETPKMessage* msg = dynamic_cast<CLCAGETPKMessage*>(vec->at(0));
		if(msg == 0)
		{
			CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForSGETPKMsg(),msg error",0);
			return ;
		}

		CLSqlite* sqlite = CLSqlite::getInstance();
		int s = sqlite->sqlPKqueryexec(msg->Name,msg->PKType);
		CLCAREGETPKMessage* reMsg = 0;
		if(s == -1)
		{
			CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForSGETPKMsg(),sqlPKqueryexec error",0);
			reMsg = new CLCAREGETPKMessage(UNSUCCESS,SQL_ERROR,0,0,msg->EchoID);
			
		}
		else
		{
			uint8_t* pk = sqlite->getPKQueryResult();
			if(pk == 0)
				reMsg = new CLCAREGETPKMessage(UNSUCCESS,NOQUERY_ERROR,0,0,msg->EchoID);
			else
				reMsg = new CLCAREGETPKMessage(SUCCESS,NO_ERROR,strlen(pk),pk,msg->EchoID);

		}

		vector<CLCAMessage*>* vec = new vector<CLCAMessage*>;
		vec->push_back(reMsg);

		oberver->WriteNetMsg(vec,hs->sock,false);
	}
	catch(char* str)
	{
		CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForSGETPKMsg(),",0);
		
	}


}

void CLServerMessageObserver::HandlerForMGETPKMsg(void* pContext)
{
	if(pContext == 0)
		return;

	try
	{
		HandlerStruct* hs = (HandlerStruct*)pContext;
		CLMessageObserver* observer = hs->observer;
		vector<CLCAMessage*>* vec = (vector<CLCAMessage*>*)hs->MsgData;

		if(vec == 0)
		{
			CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForMGETPKMsg(),vec null",0);
			return;
		}

		CLCAGETPKMessage* msg = 0;
		CLCAREGETPKMessage* remsg = 0;

		CLSqlite* sql = CLSqlite::getInstance();
		if(sql == 0)
			throw "In CLServerMessageObserver::HandlerForMGETPKMsg(),sql null";

		vector<CLCAREGETPKMessage*>* remsg_vec = new vector<CLCAREGETPKMessage*>;

		int i = 0;
		while(i< vec->size())
		{
			msg = dynamic_cast<CLCAGETPKMessage*>(vec->at(i));
			if(msg == 0)
			{
				CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForMGETPKMsg(),msg null",0);
				i++;
				continue;
			}

			int ret = sql->sqlPKqueryexec(msg->Name,msg->PKType);
			if(ret == -1)
			{
				CLLogger::WriteLogMsg("In CLServerMessageObserver::HandlerForMGETPKMsg(),sqlPKqueryexec() error",0);
				reMsg = new CLCAREGETPKMessage(UNSUCCESS,SQL_ERROR,0,0,msg->EchoID);
			}
			else
			{
				uint8_t* pk = sqlite->getPKQueryResult();
				if(pk == 0)
					reMsg = new CLCAREGETPKMessage(UNSUCCESS,NORECORD_ERROR,0,0,msg->EchoID);
				else
					reMsg = new CLCAREGETPKMessage(SUCCESS,NO_ERROR,strlen(pk),pk,msg->EchoID);
			}

			remsg_vec->push_back(remsg);
			i++;
		}

		observer->WriteNetMsg(remsg_vec,hs->sock,true,PK_FORREMGET);

	}
	catch (char* str)
	{
		CLLogger::WriteLogMsg(str,0);
	}
}

void CLServerMessageObserver::HandlerForSUPDATEMsg(void* pContext)
{
	if(pContext == 0)
		return;


}