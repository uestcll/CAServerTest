#include "CLCAUpdatePKMsgDeSerializer.h"
#include "CLCAMessage.h"
#include "CLBuffer.h"
#include "CLLogger.h"
#include "CLCAUpdatePKMessage.h"

#include <string.h>
#include <arpa/inet.h>

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
		CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),set setSuccessionIndex error",0);
		return 0;
	}

	
	bool IsDeleted = false;
	uint8_t* buf = Buf->getSuccessionBuf(24,&IsDeleted);

	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		return 0;
	}

	uint32_t* echoid = (uint32_t*)buf;
	uint32_t* lenForServerName = (uint32_t*)(buf + 4);
	uint32_t* lenForPW = (uint32_t*)(buf + 8);
	uint32_t* lenForPWAW = (uint32_t*)(buf + 12);
	uint32_t* pktype = (uint32_t*)(buf + 16);
	uint32_t* pklen = (uint32_t*)(buf + 20);

	bool first24_bufDeleted = IsDeleted;
	uint8_t* first24_buf = buf;

	buf = Buf->getSuccessionBuf(ntohl(*lenForServerName),&IsDeleted);

	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);

		if(first24_bufDeleted)
			delete first24_buf;

		return 0;
	}
	uint8_t* ServerName = buf;
	bool ServerNameDeleted = IsDeleted;

	uint32_t ReservedLen1 = ntohl(*lenForServerName) % 4 == 0 ? 0 : (4 - ntohl(*lenForServerName) % 4);

	if(ReservedLen1 != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen1,&IsDeleted);

		if(buf == 0)
		{
			CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
			if(first24_bufDeleted)
				delete first24_buf;

			if(ServerNameDeleted)
				delete ServerName;

			return 0;
		}

		if(IsDeleted)
			delete buf;

	}

	buf = Buf->getSuccessionBuf(ntohl(*lenForPW),&IsDeleted);

	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		
		if(first24_bufDeleted)
			delete first24_buf;
		
		if(ServerNameDeleted)
			delete ServerName;

		return 0;
	}

	uint8_t* PublicWord = buf;
	bool PWDeleted = IsDeleted;

	uint32_t ReservedLen2 = ntohl(*lenForPW) % 4 == 0 ? 0 : (4 - ntohl(*lenForPW) % 4);

	if(ReservedLen2 != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen2,&IsDeleted);

		if(buf == 0)
		{
			CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
			if(first24_bufDeleted)
				delete first24_buf;

			if(ServerNameDeleted)
				delete ServerName;

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
		CLLogger::WriteLogMsg("In CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		if(first24_bufDeleted)
			delete first24_buf;

		if(ServerNameDeleted)
			delete ServerName;

		if(PWDeleted)
			delete PublicWord;

		return 0;
	}

	uint8_t* passwd_PWAW = buf;
	bool passwdDeleted = IsDeleted;

	uint32_t ReservedLen3 = ntohl(*lenForPWAW) % 4 == 0 ? 0 : (4 - ntohl(*lenForPWAW) % 4);

	if(ReservedLen3 != 0)
	{
		buf = Buf->getSuccessionBuf(ReservedLen3,&IsDeleted);

		if(buf == 0)
		{
			CLLogger::WriteLogMsg("CLCAUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
			if(first24_bufDeleted)
				delete first24_buf;

			if(ServerNameDeleted)
				delete ServerName;

			if(PWDeleted)
				delete PublicWord;

			if(passwdDeleted)
				delete passwd_PWAW;

			return 0;
		}

		if(IsDeleted)
			delete buf;
	}


	CLCAMessage* msg = new CLCAUpdatePKMessage(ntohl(*echoid),ntohl(*lenForServerName),ntohl(*lenForPW),ntohl(*lenForPWAW),
		ntohl(*pktype),ntohl(*pklen),ServerName,PublicWord,passwd_PWAW);

	vector<CLCAMessage*>* vec = new vector<CLCAMessage*>;
	vec->push_back(msg);

	if(first24_bufDeleted)
		delete first24_buf;

	if(ServerNameDeleted)
		delete ServerName;

	if(PWDeleted)
		delete PublicWord;

	if(passwdDeleted)
		delete passwd_PWAW;

	return vec;
	
}