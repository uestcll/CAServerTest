#include "CLCAUpdatePKMutiMsgDeSerializer.h"
#include "CLLogger.h"
#include "CLCAMessage.h"
#include "CLBuffer.h"
#include "CLCAUpdatePKMsgDeSerializer.h"

#include <string.h>
#include <arpa/inet.h>

using namespace std;

CLCAUpdatePKMutiMsgDeSerializer::CLCAUpdatePKMutiMsgDeSerializer()
{
	SingleMsgDeser = new CLCAUpdatePKMsgDeSerializer;
	msgNum = 0;
}

CLCAUpdatePKMutiMsgDeSerializer::~CLCAUpdatePKMutiMsgDeSerializer()
{
	delete SingleMsgDeser;
}

void CLCAUpdatePKMutiMsgDeSerializer::getMsgNumber(uint8_t* buf)
{
	uint32_t* num = (uint32_t*)buf;
	msgNum = ntohl(*num);
}

vector<CLCAMessage*>* CLCAUpdatePKMutiMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	if(Buf == 0)
		return 0;

	if(-1 == Buf->setSuccessionIndex(startindex))
	{
		CLLogger::WriteLogMsg("In CLCAUpdatePKMutiMsgDeSerializer::DeSerializer(),setSuccessionIndex error",0);
		return 0;
	}

	bool IsDeleted = false;
	uint8_t* buf = Buf->getSuccessionBuf(4,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAUpdatePKMutiMsgDeSerializer::DeSerializer(),getSuccessionIndex error",0);
		if(IsDeleted)
			delete buf;

		return 0;
	}

	getMsgNumber(buf);
	if(IsDeleted)
		delete buf;

	vector<CLCAMessage*>* ret_vec = new vector<CLCAMessage*>;
	vector<CLCAMessage*>* vec = 0;
	int i = 0;

	vector<CLCAMessage*>::iterator it;
	CLCAMessage* msg = 0;
	
	while(i < msgNum)
	{
		uint32_t NextIndex = Buf->getSuccessionIndex();
		vec = SingleMsgDeser->DeSerializer(Buf,NextIndex);

		//一旦出错 ，立即返回0
		if(vec == 0)
		{
			for(it = ret_vec->begin(); it!= ret_vec->end();it++)
			{
				msg = (*it);
				if(msg != 0)
					delete msg;
			}

			delete ret_vec;
			return 0;
		}

		ret_vec->push_back(vec->at(0));
		delete vec;
		i++;
	}

	if(ret_vec->size() == 0)
	{
		delete ret_vec;
		return 0;
	}

	return ret_vec;
}

