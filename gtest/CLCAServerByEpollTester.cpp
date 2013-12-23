#include <gtest/gtest.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "HeadFile.h"

#define SERVER_PORT 39999
#define DEBUG_TEST

using namespace std;

static void startServer(void* pContext)
{
	try
	{

	CLCAServer* server = new CLCAServerByEpoll(0,SERVER_PORT);
	server->Initialize(new CLDataReceviverBySocket);
	server->run();

	delete server;
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg(str,0);
	}
}

static void* startClient(void* pContext)
{
	try
	{
	string sName = "TesterName";
	string sPk = "TesterPublicKey";
	
	char ServerName[20];
	char PublicKey[128];
	memset(ServerName,0,20);
	memset(PublicKey,0,128);

	long i = (long)pContext;

	sprintf(ServerName,"%s%ld",sName.c_str(),i * 3);
	sprintf(PublicKey,"%s%ld",sPk.c_str(),i * 3);

	CLCAPostNetMsgTool* post = new CLCAClientPostNetMsgBySocket(AF_INET,(uint8_t*)"127.0.0.1",SERVER_PORT);
	CLCAMessage* msg = new CLCAGETPKMessage(strlen(ServerName),(uint8_t*)ServerName,i * 3,0);
	post->PostSingleNetMessage(msg);

	vector<CLCAMessage*>* vec = (vector<CLCAMessage*>*)post->ReadFromNet();
	EXPECT_TRUE(vec != 0);
	EXPECT_EQ(vec->size() ,1);

	CLCAREGETPKMessage* message  = dynamic_cast<CLCAREGETPKMessage*>(vec->at(0));
	EXPECT_TRUE(message != 0);
	EXPECT_EQ(message->EchoId,0);
	EXPECT_TRUE(strcmp(PublicKey,(char*)message->PublicKey) == 0);


	delete message;
	delete vec;
//	delete msg;

	delete post;
	}
	catch(const char* str)
	{
		throw str;
	}

	return 0;
}

static void* startClientForMutiMsg(void* pContext)
{
	try
	{
		string sName = "TesterName";
		string sPk = "TesterPublicKey";

		char ServerName[20];
		char PublicKey[128];
		memset(ServerName,0,20);
		memset(PublicKey,0,128);


		CLCAMessage* msg = 0;
		CLCAGETPKMessage* pk_msg = 0;
		vector<CLCAMessage*>* msg_vec = new vector<CLCAMessage*>;
		CLCAREGETPKMessage* repk_msg = 0;

		int i = 0;
		for(;i < 1000 ; i++)
		{
			pk_msg = 0;
			msg = 0;

			memset(ServerName,0,20);
			sprintf(ServerName,"%s%d",sName.c_str(),i  * 3);

			msg = new CLCAGETPKMessage(strlen(ServerName),(uint8_t*)ServerName,3*i,i + 1000);
			EXPECT_TRUE(msg != 0);

			msg_vec->push_back(msg);
		}

		CLCAPostNetMsgTool* post = new CLCAClientPostNetMsgBySocket(AF_INET,(uint8_t*)"127.0.0.1",SERVER_PORT);
		post->PostNetMessage(msg_vec,PK_FORMGET);

		msg_vec = (vector<CLCAMessage*>*)post->ReadFromNet();

		EXPECT_TRUE(msg_vec != 0);
		EXPECT_EQ(msg_vec->size(),1000);

		for(i = 0; i<msg_vec->size() ; i++)
		{
			msg = msg_vec->at(i);

			memset(PublicKey,0,128);
			sprintf(PublicKey,"%s%d",sPk.c_str(),i * 3);

			repk_msg = dynamic_cast<CLCAREGETPKMessage*>(msg);
			EXPECT_TRUE(repk_msg != 0);

			EXPECT_EQ(repk_msg->EchoId , i + 1000);
			EXPECT_TRUE(strcmp(PublicKey,(char*)repk_msg->PublicKey) == 0);

			delete msg;
		}

		delete msg_vec;
		delete post;
	}
	catch(const char* str)
	{
		throw str;
	}

	return 0;
}

void sqlRecordInit()
{
	CLSqlite* sql = CLSqlite::getInstance();

	string sName = "TesterName";
	string sPk = "TesterPublicKey";

	char ServerName[20];
	char PublicKey[128];

	int ret = -1;
	int i ;

	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i * 3);
		sprintf(PublicKey,"%s%d",sPk.c_str(),i * 3);

		ret = sql->sqlPKinsertexec((uint8_t*)ServerName,i * 3,(uint8_t*)PublicKey);

		EXPECT_EQ(ret,0);
	}
}

void sqlUninit()
{
	CLSqlite* sql = CLSqlite::getInstance();
	sql->sqlDeleteAllRecords();
}

TEST(CLCAServerByEpoll,SqlInit)
{
	sqlRecordInit();
}

TEST(CLCAServerByEpoll,SingleThreadSingleMsgTest)
{
	try
	{
		startClient((void*)0);
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg(str,0);
	}
}

TEST(CLCAServerByEpoll,SingleThreadMutiMsgTest)
{
	try
	{
		startClientForMutiMsg(0);
	}
	catch (const char* str)
	{
		cout<<str<<endl;
	}
}

TEST(CLCAServerByEpoll,NormalTest)
{
	pthread_t pid ;
	pthread_t tid;
	try
	{
		int i ;
		for(i = 0; i< 100 ; i++)
		{
			pthread_create(&pid,0,startClient,(void*)i);
			pthread_join(pid,0);

			pthread_create(&tid,0,startClientForMutiMsg,(void*)i);
			pthread_join(tid,0);
		}

	}
	catch(const char* str)
	{
		cout<<str<<endl;
	}


}

TEST(CLCAServerByEpoll,SqlUninit)
{
	sqlUninit();
}