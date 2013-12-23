#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "CLBuffer.h"

using namespace std;

CLBuffer* InitBuffer()
{
	CLBuffer* m_buf = new CLBuffer;
	int i = 0;
	string s = "getbuftest";
	char inc[20];

	while(i<1000)
	{
		memset(inc,0,20);
		sprintf(inc,"%s%d",s.c_str(),i);
		m_buf->addBuf((uint8_t*)inc,strlen(inc),true);

		i++;
	}

	return m_buf;
}


TEST(CLBuffer,GetBufMutiNormalTest)
{
	CLBuffer* m_buf = InitBuffer();
	string s = "getbuftest";

	int i = 0;
	char temp[20];
	uint32_t index = 0;
	uint8_t* buf = 0;
	bool IsDelete = false;

	while(i<1000)
	{
		buf = 0;
		memset(temp,0,20);
		sprintf(temp,"%s%d",s.c_str(),i);

		buf = m_buf->getBuf(index,strlen(temp),&IsDelete);
		EXPECT_TRUE(buf != 0);

		if(buf != 0)
		{
			EXPECT_TRUE(strcmp((char*)buf,temp) == 0);
			if(IsDelete)
				delete buf;
		}

		index += strlen(temp);

		i++;
 
	}

	delete m_buf;


}

TEST(CLBuffer,GetBufMutiStepTest)
{
	CLBuffer* m_buf = InitBuffer();
	string s = "getbuftest";

	int i = 0;
	char temp[60];
	uint32_t index = 0;
	uint8_t* buf = 0;
	bool IsDelete = false;

	index += s.length();

	while(i<999)
	{
		buf = 0;
		memset(temp,0,60);
		sprintf(temp,"%d%s%d%s%d%s",i,s.c_str(),i+1,s.c_str(),i+2,s.c_str());
		i += 3;


		buf = m_buf->getBuf(index,strlen(temp),&IsDelete);
		EXPECT_TRUE(buf != 0);

		if(buf != 0)
		{
			EXPECT_TRUE(strcmp((char*)buf,temp) == 0);
			if(IsDelete)
				delete buf;
		}

		index += strlen(temp);

	}

	delete m_buf;
}

TEST(CLBuffer,GetSuccessionBufNormalTest)
{
	CLBuffer* m_buf = InitBuffer();
	string s = "getbuftest";

	int i = 0;
	char temp[20];

	uint32_t index = 0;
	uint8_t* buf = 0;
	bool IsDelete = false;

	EXPECT_EQ(m_buf->setSuccessionIndex(0),0);

	while(i<1000)
	{
		buf = 0;
		memset(temp,0,20);
		sprintf(temp,"%s%d",s.c_str(),i);

		buf = m_buf->getSuccessionBuf(strlen(temp),&IsDelete);
		
		EXPECT_TRUE(buf != 0);

		if(buf != 0)
		{
			EXPECT_TRUE(strcmp((char*)buf,temp) == 0);
	
			if(IsDelete)
				delete buf;
		}

		index += strlen(temp);

		EXPECT_EQ(index,m_buf->getSuccessionIndex());

		i++;
	}

	i = 1;
	int j = 998;
	char lasttemp[20];
	index = 0;

	EXPECT_EQ(m_buf->setSuccessionIndex(0),0);

	uint32_t fulllen = m_buf->getBufferSize();
	index = fulllen;

	index -= 3;

	while(j >= 0)
	{
		buf = 0;
		memset(temp,0,20);
		sprintf(temp,"%d%s",j,s.c_str());

		index -= strlen(temp);

		EXPECT_EQ(m_buf->setSuccessionIndex(index),0);

		buf = m_buf->getSuccessionBuf(strlen(temp),&IsDelete);

		EXPECT_TRUE(buf != 0);

		if(buf != 0)
		{
			EXPECT_TRUE(strcmp((char*)buf,temp) == 0);

			if(IsDelete)
				delete buf;
		}

		j--;
	}

	delete m_buf;
}

TEST(CLBuffer,GetSuccessionBufStepTest)
{
	CLBuffer* m_buf = InitBuffer();
	string s = "getbuftest";

	int i = 0;
	char temp[60];
	uint32_t index = 0;
	uint8_t* buf = 0;
	bool IsDelete = false;

	index += s.length();

	EXPECT_EQ(m_buf->setSuccessionIndex(index),0);

	while(i < 999)
	{
		buf = 0;
		memset(temp,0,60);
		sprintf(temp,"%d%s%d%s%d%s",i,s.c_str(),i+1,s.c_str(),i+2,s.c_str());
		i += 3;

		buf = m_buf->getSuccessionBuf(strlen(temp),&IsDelete);
	
		EXPECT_TRUE(buf != 0);

		if(buf != 0)
		{
			EXPECT_TRUE(strcmp((char*)buf,temp) == 0);

			if(IsDelete)
				delete buf;
		}

		index += strlen(temp);

		EXPECT_EQ(index,m_buf->getSuccessionIndex());

	}

	delete m_buf;
}
