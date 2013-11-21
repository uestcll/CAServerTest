#include "CLCAREUpdatePKMsgDeSerializer.h"
#include "CLLogger.h"
#include <sys/socket.h>

using namespace std;


CLCAREUpdatePKMsgDeSerializer::CLCAREUpdatePKMsgDeSerializer()
{

}

CLCAREUpdatePKMsgDeSerializer::~CLCAREUpdatePKMsgDeSerializer()
{

}

vector<CLCAMessage*>* CLCAREUpdatePKMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	if(Buf == 0)
		return 0;

	if(-1 == Buf->setSuccessionIndex(startindex))
	{
		CLLogger::WriteLogMsg("In CLCAREUpdatePKMsgDeSerializer::DeSerializer(),setSuccessionIndex error",0);
		return 0;
	}

	bool IsDeleted = false;
	uint8_t* buf = Buf->getSuccessionBuf(8,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLogMsg("In CLCAREUpdatePKMsgDeSerializer::DeSerializer(),getSuccessionBuf error",0);
		return 0;
	}

	uint16_t* isSuccess = (uint16_t*)buf;
	uint16_t* errorno = (uint16_t*)(buf + 2);
	uint32_t* echoId = (uint32_t*)(buf + 4);

	CLCAREUpdatePKMessage* msg = new CLCAREUpdatePKMessage(ntohs(*isSuccess),ntohs(*errno),ntohl(*echoId));
	vector<CLCAMessage*>* vec = new vector<CLCAMessage*>;
	vec->push_back(msg);

	if(IsDeleted)
		delete buf;

	return vec;

}