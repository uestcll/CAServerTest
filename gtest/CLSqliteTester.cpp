#include <gtest/gtest.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "HeadFile.h"

using namespace std;



TEST(CLSqlite, TestNormal)
{

	string sName = "TesterName";
	string sPk = "TesterPublicKey";

	CLSqlite* sql = CLSqlite::getInstance();
	EXPECT_TRUE(sql != 0);

	char ServerName[20];
	char PublicKey[128];

	int ret = -1;
	int i ;

	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i);
		sprintf(PublicKey,"%s%d",sPk.c_str(),i);

		ret = sql->sqlPKinsertexec((uint8_t*)ServerName,i,(uint8_t*)PublicKey);

		EXPECT_EQ(ret,0);
	}

	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i);
		sprintf(PublicKey,"%s%d",sPk.c_str(),i);

		ret = sql->sqlPKinsertexec((uint8_t*)ServerName,i,(uint8_t*)PublicKey);

		EXPECT_EQ(ret,-1);
	}

	uint8_t* pk = 0;

	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i);
		sprintf(PublicKey,"%s%d",sPk.c_str(),i);

		ret = sql->sqlPKqueryexec((uint8_t*)ServerName,i,&pk);

		EXPECT_EQ(ret,0);
		EXPECT_TRUE(strcmp(PublicKey,(char*)pk) == 0);
		
		if(pk != 0)
			delete pk;
	}


	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i);
		sprintf(PublicKey,"%s%d",sPk.c_str(),(i + 1) * 2);

		ret = sql->sqlPKupdateexec((uint8_t*)ServerName,i,(uint8_t*)PublicKey);

		EXPECT_EQ(ret,0);
	}

	for( i = 0 ; i < 1000 ; i++)
	{
		memset(ServerName,0,20);
		memset(PublicKey,0,128);

		sprintf(ServerName,"%s%d",sName.c_str(),i);
		sprintf(PublicKey,"%s%d",sPk.c_str(),2 * (i+1));

		ret = sql->sqlPKqueryexec((uint8_t*)ServerName,i,&pk);

		EXPECT_EQ(ret,0);
		EXPECT_TRUE(strcmp(PublicKey,(char*)pk) == 0);

		if(pk != 0)
			delete pk;
	}

	sql->sqlDeleteAllRecords();

}