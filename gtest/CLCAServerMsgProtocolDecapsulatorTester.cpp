#include <gtest/gtest.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>

#include "HeadFile.h"

using namespace std;

string s = "TesterStrs";

struct PTestSt
{
	uint8_t* Buf;
	uint32_t Size;
	uint32_t Expect_LeftSize;
};

void ProtocolTesterInit()
{
	
}

void ProtocolTesterUninit()
{
	
}

CLCAMessage* getSingleMessage(int n)
{
	char str[20];
	memset(str,0,20);


	sprintf(str,"%s%d",s.c_str(),n);
	CLCAMessage* msg = new CLCAGETPKMessage(strlen(str),(uint8_t*)str,n * 2,n);

	return msg;
}

vector<CLCAMessage*>* getMutiMessage(int f , int e)
{
	char str[20];

	CLCAMessage* msg = 0;
	vector<CLCAMessage*>* vec = new vector<CLCAMessage*>;

	for(int i = f ;i <= e ; i++)
	{
		memset(str,0,20);
		sprintf(str,"%s%d",s.c_str(),i);

		msg = new CLCAGETPKMessage(strlen(str),(uint8_t*)str,i * 2,i);
		vec->push_back(msg);
	}

	return vec;
}

void ReleseVec(vector<CLCAMessage*>* vec)
{
	CLCAMessage* msg = 0;

	for(int i = 0; i <vec->size() ; i++)
	{
		msg = vec->at(i);
		delete msg;
	}

	delete vec;
}

template<typename T>
void freeVec(vector<T*>* vec)
{
	T* e = 0;
	for(int i = 0; i< vec->size() ; i++)
	{
		e = vec->at(i);
		delete e;
	}

	delete vec;
}

vector<PTestSt*>* getNormalTestBuf()
{
	PTestSt* m_st = 0;
	CLCAMessage* msg = 0;
	vector<CLCAMessage*>* vec = 0;
	
	vector<PTestSt*>* ret_vec = new vector<PTestSt*>;

	CLCASerializer* ser = new CLCAGETPKMsgSerializer;
	uint32_t Size = 0;

	for(int i = 0 ;i< 10000 ; i++)
	{
		m_st = (PTestSt*)malloc(sizeof(PTestSt));

		msg = getSingleMessage(i);
		m_st->Buf = ser->Serialize(msg,0,0,&Size);
		m_st->Size = Size;
		m_st->Expect_LeftSize = 0;

		ret_vec->push_back(m_st);
		
	}

	delete ser;
	return ret_vec;
};

//Size >= 100,parts >= 5
void DivideIntoParts(uint8_t* buf , uint32_t Size , vector<PTestSt*>* vec , int parts)
{
	PTestSt* st = 0;
	uint32_t index = 0;
	uint8_t* m_buf = 0;
	uint32_t buf_size = 0;

	for(int i = 0 ;i < parts - 1 ; i++)
	{
		st = (PTestSt*)malloc(sizeof(PTestSt));

		if(i < 2)
		{
			m_buf = new uint8_t[6];
			memset(m_buf , 0 ,6);
			memcpy(m_buf ,buf + index ,5 );

			index += 5;
			Size -= 5;

			st->Buf = m_buf;
			st->Size = 5;
			st->Expect_LeftSize = Size;

			if(i == 0)
				st->Expect_LeftSize = 0xffffffff;

			vec->push_back(st);

			buf_size = Size % (parts - 1);
			continue;
		}

		
		m_buf = new uint8_t[buf_size + 1];
		memset(m_buf,0,buf_size + 1);

		memcpy(m_buf , buf + index , buf_size);
		index += buf_size;
		Size -= buf_size;

		st->Buf = m_buf;
		st->Size = buf_size;
		st->Expect_LeftSize = Size;

		vec->push_back(st);

	}

	st = (PTestSt*)malloc(sizeof(PTestSt));
	buf_size = Size ;

	m_buf = new uint8_t[buf_size + 1];
	memset(m_buf,0,buf_size + 1);

	memcpy(m_buf , buf + index , buf_size);
	index += buf_size;
	Size -= buf_size;

	st->Buf = m_buf;
	st->Size = buf_size;
	st->Expect_LeftSize = Size;

	vec->push_back(st);

	delete buf;
}

vector<PTestSt*>* getInNormalTestBuf()
{
	PTestSt* m_st = 0;
	CLCAMessage* msg = 0;
	vector<CLCAMessage*>* vec = 0;

	vector<PTestSt*>* ret_vec = new vector<PTestSt*>;

	CLCASerializer* ser = new CLCAGETPKMutiMsgSerializer;

	uint8_t* buf = 0;
	uint32_t buf_size = 0;

	for(int i = 0 ;i < 10000 ; i++)
	{

		vec = getMutiMessage(i,i+9);
		buf = ser->Serialize(0,vec,0,&buf_size);
		DivideIntoParts(buf,buf_size,ret_vec,10);

		i += 9;
	}

	return ret_vec;
}



TEST(CLCAServerMsgProtocolDecapsulator,NormalTest)
{
	ProtocolTesterInit();

	PTestSt* st = 0;
	char str[20];
	CLCADeSerializer* deSer = new CLCAGETPKMsgDeSerializer;

	CLCAMessage* message = 0;
	CLCAGETPKMessage* msg = 0;
	vector<CLCAMessage*>* msg_vec = 0;

	vector<PTestSt*>* vec = getNormalTestBuf();

	CLProtocolDecapsulator* pd = new CLCAServerMsgProtocolDecapsulator;
	EXPECT_EQ(pd->getLeftSize() , 0xffffffff);

	vector<CLBuffer*>* vec_buffer = 0;
	CLBuffer* buffer = 0;

	for(int i = 0 ;i < vec->size() ; i++)
	{

		memset(str,0,20);
		sprintf(str,"%s%d",s.c_str(),i);

		st = vec->at(i);
		pd->addCharToDecapsulate(st->Buf,st->Size);

		pd->ProtocolDecapsulate();

		vec_buffer = pd->getDecapsulatorMsgChar();

		EXPECT_TRUE(vec_buffer != 0);
		EXPECT_EQ(vec_buffer->size(),1);

		EXPECT_EQ(pd->getLeftSize(),st->Expect_LeftSize);

		buffer = vec_buffer->at(0);
		
		msg_vec = deSer->DeSerializer(buffer,8);
		EXPECT_TRUE(msg_vec != 0);
		EXPECT_TRUE(msg_vec->size() == 1);

		message = msg_vec->at(0);
		msg = dynamic_cast<CLCAGETPKMessage*>(message);

		EXPECT_TRUE(msg != 0);
		EXPECT_EQ(strlen(str),msg->LengthOfName);
		EXPECT_TRUE(strcmp(str,(char*)msg->Name) == 0);
		EXPECT_EQ(msg->PKType,2 * i);
		EXPECT_EQ(msg->EchoID,i);

		delete buffer;
		delete vec_buffer;
		delete message;
		delete msg_vec;

		free(st);
	}

	delete vec;
	delete deSer;
	delete pd;

	ProtocolTesterUninit();
}

TEST(CLCAServerMsgProtocolDecapsulator,InNormalTest)
{
	ProtocolTesterInit();

	PTestSt* st = 0;
	vector<PTestSt*>* vec = getInNormalTestBuf();
	vector<CLBuffer*>* vec_buffer = 0;

	CLProtocolDecapsulator* pd = new CLCAServerMsgProtocolDecapsulator;
	EXPECT_EQ(pd->getLeftSize(),0xffffffff);

	for(int i = 0 ;i < vec->size() ; i++)
	{
		st = vec->at(i);
		pd->addCharToDecapsulate(st->Buf,st->Size);
		pd->ProtocolDecapsulate();

	//	freeVec<CLBuffer>(vec_buffer);

		EXPECT_EQ(pd->getLeftSize(),st->Expect_LeftSize);

		free(st);

	}

	delete vec;
	delete pd;

	ProtocolTesterUninit();


}