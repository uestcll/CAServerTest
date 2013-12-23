#include <gtest/gtest.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#include "HeadFile.h"

using namespace std;

void SerializerTesterInit()
{
	
}

void SerializerTesterUninit()
{
	
}

TEST(CLCAGETPKMessage,SerializerAndDeSerializerSingleTester)
{
	SerializerTesterInit();

	char str[20];
	string s = "TesterName";

	CLCAMessage* msg = 0;
	CLCAGETPKMessage* pk_msg = 0;
	vector<CLCAMessage*>* vec = 0;

	CLCASerializer* ser = new CLCAGETPKMsgSerializer;
	CLCADeSerializer* deSer = new CLCAGETPKMsgDeSerializer;

	uint8_t* Ser_buf = 0;
	uint32_t Ser_size = 0;

	int i = 0;
	for(;i < 10000 ; i++)
	{
		pk_msg = 0;
		msg = 0;

		memset(str,0,20);
		sprintf(str,"%s%d",s.c_str(),i);

		msg = new CLCAGETPKMessage(strlen(str),(uint8_t*)str,2*i,i);
		EXPECT_TRUE(msg != 0);

		if(msg != 0)
		{
			Ser_buf = ser->Serialize(msg,0,0,&Ser_size);

			EXPECT_TRUE(Ser_buf != 0);
			EXPECT_TRUE(Ser_size != 0);

			msg = 0;
		}

		vec = deSer->DeSerializer((uint8_t*)(Ser_buf + 8) ,Ser_size - 8);

		if(Ser_buf != 0)
			delete Ser_buf;

		EXPECT_TRUE(vec != 0);
		EXPECT_TRUE(vec->size() == 1);

		msg = vec->at(0);
		

		pk_msg = dynamic_cast<CLCAGETPKMessage*>(msg);

		EXPECT_TRUE(pk_msg != 0);

		EXPECT_TRUE(strcmp((char*)(pk_msg->Name),str) == 0);
		EXPECT_EQ(pk_msg->LengthOfName , strlen(str));
		EXPECT_EQ(pk_msg->PKType , 2 * i);
		EXPECT_EQ(pk_msg->EchoID , i);

		delete msg;
		delete vec;
	}

	delete ser;
	delete deSer;

	SerializerTesterUninit();
}

TEST(CLCAGETPKMessage,SerializerAndDeSerializerMutiTester)
{
	SerializerTesterInit();

	char str[20];
	string s = "TesterName";

	CLCAMessage* msg = 0;
	CLCAGETPKMessage* pk_msg = 0;
	vector<CLCAMessage*>* msg_vec = new vector<CLCAMessage*>;

	CLCASerializer* ser = new CLCAGETPKMutiMsgSerializer;
	CLCADeSerializer* deSer = new CLCAGETPKMutiMsgDeSerializer;

	uint8_t* Ser_buf = 0;
	uint32_t Ser_size = 0;

	int i = 0;
	for(;i < 1000 ; i++)
	{
		pk_msg = 0;
		msg = 0;

		memset(str,0,20);
		sprintf(str,"%s%d",s.c_str(),i);

		msg = new CLCAGETPKMessage(strlen(str),(uint8_t*)str,2*i,i);
		EXPECT_TRUE(msg != 0);

		msg_vec->push_back(msg);
	}

	Ser_buf = ser->Serialize(0,msg_vec,0,&Ser_size);

	EXPECT_TRUE(Ser_buf != 0);


	vector<CLCAMessage*>* vec = deSer->DeSerializer((uint8_t*)(Ser_buf + 8),Ser_size - 8);

	delete Ser_buf;

	EXPECT_TRUE(vec != 0);
	EXPECT_EQ(vec->size(),1000);

	for(i = 0; i< 1000 ;i++)
	{
		pk_msg = 0;
		msg = 0;

		memset(str,0,20);
		sprintf(str,"%s%d",s.c_str(),i);

		msg = vec->at(i);
		pk_msg = dynamic_cast<CLCAGETPKMessage*>(msg);

		EXPECT_TRUE(pk_msg != 0);

		EXPECT_TRUE(strcmp((char*)(pk_msg->Name),str) == 0);
		EXPECT_EQ(pk_msg->LengthOfName , strlen(str));
		EXPECT_EQ(pk_msg->PKType , 2 * i);
		EXPECT_EQ(pk_msg->EchoID , i);

		delete msg;
	}

	delete vec;

	delete ser;
	delete deSer;


	SerializerTesterUninit();
}

TEST(CLCAUpdatePKMessage,SerializerAndDeSerializerSingleTester)
{
	SerializerTesterInit();

	char str1[20];
	char str2[20];
	char str3[20];

	string s1 = "TesterName";
	string s2 = "UpdateTester";
	string s3 = "TestedPKAW";

	CLCAMessage* msg = 0;
	CLCAUpdatePKMessage* pk_msg = 0;
	vector<CLCAMessage*>* vec = 0;

	CLCASerializer* ser = new CLCAUpdatePKMsgSerializer;
	CLCADeSerializer* deSer = new CLCAUpdatePKMsgDeSerializer;

	uint8_t* Ser_buf = 0;
	uint32_t Ser_size = 0;

	int i = 0;
	for(;i < 10000 ; i++)
	{
		pk_msg = 0;
		msg = 0;

		memset(str1,0,20);
		memset(str2,0,20);
		memset(str3,0,20);

		sprintf(str1,"%s%d",s1.c_str(),i);
		sprintf(str2,"%s%d",s2.c_str(),i);
		sprintf(str3,"%s%d",s3.c_str(),i);

		msg = new CLCAUpdatePKMessage(i,strlen(str1),strlen(str2),strlen(str3),2*i,3*i,(uint8_t*)str1,(uint8_t*)str2,(uint8_t*)str3);
		EXPECT_TRUE(msg != 0);

		if(msg != 0)
		{
			Ser_buf = ser->Serialize(msg,0,0,&Ser_size,true,false);

			EXPECT_TRUE(Ser_buf != 0);
			EXPECT_TRUE(Ser_size != 0);

//			delete msg;
			msg = 0;
		}

		vec = deSer->DeSerializer(Ser_buf  ,Ser_size );

		if(Ser_buf != 0)
			delete Ser_buf;

		EXPECT_TRUE(vec != 0);
		EXPECT_TRUE(vec->size() == 1);

		msg = vec->at(0);


		pk_msg = dynamic_cast<CLCAUpdatePKMessage*>(msg);

		EXPECT_TRUE(pk_msg != 0);

		EXPECT_TRUE(strcmp((char*)(pk_msg->ServerName),str1) == 0);
		EXPECT_TRUE(strcmp((char*)(pk_msg->PublicWord),str2) == 0);
		EXPECT_TRUE(strcmp((char*)(pk_msg->Passwd_PKAW),str3) == 0);
		EXPECT_EQ(pk_msg->LenForServerName , strlen(str1));
		EXPECT_EQ(pk_msg->LenForPW , strlen(str2));
		EXPECT_EQ(pk_msg->LenForPKAW , strlen(str3));
		EXPECT_EQ(pk_msg->PKType , 2 * i);
		EXPECT_EQ(pk_msg->EchoId , i);
		EXPECT_EQ(pk_msg->PKLen , 3 * i);

		delete msg;
		delete vec;
	}

	delete ser;
	delete deSer;

	SerializerTesterUninit();
}