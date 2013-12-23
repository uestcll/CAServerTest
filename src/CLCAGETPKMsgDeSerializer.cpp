#include "CLCAGETPKMsgDeSerializer.h"
#include "CLCAGETPKMessage.h"
#include "CLLogger.h"
#include "CLCAMessage.h"
#include "CLBuffer.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

CLCAGETPKMsgDeSerializer::CLCAGETPKMsgDeSerializer()
{

}

CLCAGETPKMsgDeSerializer::~CLCAGETPKMsgDeSerializer()
{

}

vector<CLCAMessage*>* CLCAGETPKMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	
	if( -1 == Buf->setSuccessionIndex(startindex))
	{
		CLLogger::WriteLogMsg("In CLCAGETPKMsgDeSerializer::DeSerializer(),Buf setSuccessionIndex error",0);
		return 0;
	}

	bool IsDeleted = false;
	uint8_t* buf = Buf->getSuccessionBuf(4,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
		return 0;
	}

	uint32_t* LengthOfName = (uint32_t*)buf;
	uint32_t LengthOfNamed = ntohl(*LengthOfName);

	if(IsDeleted)
		delete buf;

	buf = Buf->getSuccessionBuf(LengthOfNamed,&IsDeleted);

	if( buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
		return 0;
	}

	bool NameDeleted = IsDeleted;
	uint8_t* Name = buf;
	
	int strl = (LengthOfNamed)%4 == 0?0:(4- (LengthOfNamed%4));

	if(strl != 0)
	{
		buf = Buf->getSuccessionBuf(strl,&IsDeleted);

		if( buf == 0 )
		{
			if(NameDeleted)
				delete Name;

			CLLogger::WriteLogMsg("In CLCAGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
			return 0;
		}

		if(IsDeleted)
			delete buf;

	}

	buf = Buf->getSuccessionBuf(8 ,&IsDeleted);

	if( buf == 0)
	{
		if(NameDeleted)
			delete Name;
		
		CLLogger::WriteLogMsg("In CLCAGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
		return 0;
	}

	uint32_t* pkType = (uint32_t*)buf;
	uint32_t* echoId = (uint32_t*)(buf+4);

	CLCAMessage* msg = new CLCAGETPKMessage(LengthOfNamed,Name,ntohl(*pkType),ntohl(*echoId));

	if(NameDeleted)
		delete Name;
	if(IsDeleted)
		delete buf;

	vector<CLCAMessage*>* ret_vec = new vector<CLCAMessage*>;
	ret_vec->push_back(msg);
	return ret_vec;
}