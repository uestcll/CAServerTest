#include "HeadFile.h"
#define SERVER_PORT 39999

void sqlInit()
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

void sqlUnInit()
{
	CLSqlite* sql = CLSqlite::getInstance();
	sql->sqlDeleteAllRecords();
}

int main()
{
	try
	{
	
	CLLogger::Create();

	CLSqlite::Create();
	
	sqlInit();
	CLCAServer* server = new CLCAServerByEpoll(0,SERVER_PORT);
	server->Initialize(new CLDataReceviverBySocket);
	server->run();

	delete server;
	}
	catch(const char* str)
	{
		CLLogger::WriteLogMsg(str,0);
	}

	sqlUnInit();

	CLSqlite::Destroy();
	CLLogger::Destroy();
	return 0;
}