#include "CLCAREGETPKMsgDeSerializer.h"
#include "CLCAREGETPKMessage.h"
#include "CLLogger.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

using namespace std;

CLCAREGETPKMsgDeSerializer::CLCAREGETPKMsgDeSerializer()
{

}

CLCAREGETPKMsgDeSerializer::~CLCAREGETPKMsgDeSerializer()
{

}

vector<CLCAMessage*>* CLCAREGETPKMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	if(Buf == 0)
		return 0;

	if( -1 == Buf->setSuccessionIndex(startindex))
	{
		CLLogger::WriteLog("In CLCAREGETPKMsgDeSerializer::DeSerializer(),Buf setSuccessionIndex error",0);
		return 0;
	}

	bool IsDeleted = false;
	uint8_t* buf = Buf->getSuccessionBuf(12,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLog("In CLCAREGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
		return 0;
	}

	uint16_t* isSuccess = (uint16_t*)buf;
	uint16_t* errorno = (uint16_t*)(buf+2);
	uint32_t* len = (uint32_t*)(buf+4);
	uint32_t* echoId = (uint32_t*)(buf+8);

	uint8_t* first_12 = buf;
	bool first_12Deleted = IsDeleted;

	buf = Buf->getSuccessionBuf(ntohl(*len),&IsDeleted);
	if(buf == 0)
	{
		if(first_12Deleted)
			delete first_12;

		CLLogger::WriteLog("In CLCAREGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
		return 0;
	}

	uint8_t* pk = buf;
	bool pkDeleted = IsDeleted;

	uint32_t ReservedLen = ntohl(*len) % 4 == 0 ? 0 : (4 - ntohl(*len) % 4);

	if(ReservedLen != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen,&IsDeleted);
		if(buf == 0)
		{
			CLLogger::WriteLog("In CLCAREGETPKMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf error",0);
			if(first_12Deleted)
				delete first_12;

			if(pkDeleted)
				delete pk;

			return 0;

		}

		if(IsDeleted)
			delete buf;

		
	}

	CLCAMessage* msg =  new CLCAREGETPKMessage(ntohs(*isSuccess),ntohs(*errorno),ntohl(*len),pk,ntohl(*echoId));
	
	if(first_12Deleted)
		delete first_12;
	if(pkDeleted)
		delete pk;
	
	vector<CLCAMessage*>* ret_vec = new vector<CLCAMessage*>;
	ret_vec->push_back(msg);
	return ret_vec;
}