#include <gtest/gtest.h>
#include "HeadFile.h"
#include <pthread.h>
#include <string.h>

#define GETSPKMsg_Debug
#define SERVERIP "127.0.0.1"
#define SERVERPORT 8080

void* TestThreadForCAServerReceviveSingleMsgFromClient(void* arg)
{
	sleep(10);
	CLCAPostNetMsgTool* postTool = new CLCAClientPostNetMsgBySocket(AF_INET,(uint8_t*)SERVERIP,SERVERPORT);
	CLCAGETPKMessage* msg = 0;
	const char* name = "TestServer";
	char* Tname = new char[20];
	CLCASerializer* ser = 0;
	uint8_t* buf = 0;
	for(int i = 0;i<100;i++)
	{
		ser = new CLCAGETPKMsgSerializer;
		memset(Tname,0,20);
		sprintf(Tname,"%s%d",name,i);
		msg = new CLCAGETPKMessage(strlen(Tname)+1,(uint8_t*)Tname,1,i);
		ser->Serialize(msg);
		ser->SerializeHead(PK_FORSGET,1);
		buf = ser->getSerializeChar();
		postTool->PostNetMessage(buf,ser->getFullLength());
		delete msg;
		msg = 0;
		delete buf;
		delete ser;

	}
	delete postTool;
	delete Tname;
}

TEST(CLCAServer,ReceviveSingleMsgFromClient)
{
	pthread_t pid;
	pthread_create(&pid,0,TestThreadForCAServerReceviveSingleMsgFromClient,0);
	pthread_join(pid,0);
	CLCAServerManager* manager = new CLCAServerManager(new CLCAServerByEpoll(0,SERVERPORT),new CLServerMessageObserver);
	manager->Initialize();
	manager->RunLoop();
	delete manager;
}