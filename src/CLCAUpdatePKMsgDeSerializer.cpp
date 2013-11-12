#include "CLCAUpdatePKMsgDeSerializer.h"
#include "CLLogger.h"
#include "CLCAUpdatePKMessage.h"

#include <string.h>
#include <sys/socket.h>

using namespace std;

CLCAUpdatePKMsgDeSerializer::CLCAUpdatePKMsgDeSerializer()
{

}

CLCAUpdatePKMsgDeSerializer::~CLCAUpdatePKMsgDeSerializer()
{

}

vector<CLCAMessage*>* CLCAUpdatePKMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	if(Buf == 0)
		return 0;

	if(Buf->setSuccessionIndex(startindex) == -1)
	{
		CLLogger::WriteLog("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),set setSuccessionIndex error",0);
		return 0;
	}

	
	bool IsDeleted = false;
	uint8_t buf = Buf->getSuccessionBuf(20,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLog("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		return 0;
	}

	uint32_t* echoid = (uint32_t*)buf;
	uint32_t* lenForPW = (uint32_t*)(buf + 4);
	uint32_t* lenForPWAW = (uint32_t*)(buf + 8);
	uint32_t* pktype = (uint32_t*)(buf + 12);
	uint32_t* pklen = (uint32_t*)(buf + 16);

	bool first20_bufDeleted = IsDeleted;
	uint8_t* first20_buf = buf;

	buf = Buf->getSuccessionBuf(*lenForPW,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLog("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		
		if(first20_bufDeleted)
			delete first20_buf;

		return 0;
	}

	uint8_t* PublicWord = buf;
	bool PWDeleted = IsDeleted;

	uint32_t ReservedLen1 = ntohl(*lenForPW) % 4 == 0 ? 0 : (4 - ntohl(*lenForPW) % 4);
	if(ReservedLen1 != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen1,&IsDeleted);
		if(buf == 0)
		{
			CLLogger::WriteLog("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
			if(first20_bufDeleted)
				delete first20_buf;

			if(PWDeleted)
				delete PublicWord;

			return 0;
		}

		if(IsDeleted)
			delete buf;

	}

	buf = Buf->getSuccessionBuf(ntohl(*lenForPWAW),&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLog("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		if(first20_bufDeleted)
			delete first20_buf;

		if(PWDeleted)
			delete PublicWord;

		return 0;
	}

	uint8_t* passwd_PWAW = buf;
	bool passwdDeleted = IsDeleted;

	uint32_t ReservedLen2 = ntohl(*lenForPWAW) % 4 == 0 ? 0 : (4 - ntohl(*lenForPWAW) % 4);
	if(ReservedLen2 != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen2,&IsDeleted);
		if(buf == 0)
		{
			CLLogger::WriteLog("CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
			if(first20_bufDeleted)
				delete first20_buf;

			if(PWDeleted)
				delete PublicWord;

			if(passwdDeleted)
				delete passwd_PWAW;

			return 0;
		}

		if(IsDeleted)
			delete buf;
	}


	CLCAUpdatePKMessage* msg = new CLCAUpdatePKMessage(ntohl(*echoid),ntohl(*lenForPW),ntohl(*lenForPWAW),
		ntohl(*pktype),ntohl(*pklen),PublicWord,passwd_PWAW);

	vector<CLCAMessage*>* vec = new vector<CLCAMessage*>;
	vec->push_back(msg);

	if(first20_bufDeleted)
		delete first20_buf;

	if(PWDeleted)
		delete PublicWord;

	if(passwdDeleted)
		delete passwd_PWAW;

	return vec;
	
}