#include "CLCAREUpdatePKMsgSerializer.h"
#include <sys/socket.h>
#include <string.h>

CLCAREUpdatePKMsgSerializer::CLCAREUpdatePKMsgSerializer()
{
	HeadBuf = 0;
	MsgBuf = 0;
}

CLCAREUpdatePKMsgSerializer::~CLCAREUpdatePKMsgSerializer()
{
	if(HeadBuf != 0)
		delete HeadBuf;

	if(MsgBuf != 0)
		delete MsgBuf;

}

void CLCAREUpdatePKMsgSerializer::SerializeHead(uint32_t Type,uint32_t number)
{
	if(HeadBuf != 0)
		delete HeadBuf;

	HeadBuf = new uint8_t[8];
	memset(HeadBuf,0,8);

	uint32_t* type = (uint32_t*)HeadBuf;
	*type = htonl(Type);

	uint32_t* len = (uint32_t*)(HeadBuf + 4);
	*len = htonl(FullLength);
}

uint8_t* CLCAREUpdatePKMsgSerializer::Serialize(CLCAMessage* message)
{
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;
	}

	CLCAREUpdatePKMessage* msg = dynamic_cast<CLCAREUpdatePKMessage*>message;
	if(msg == 0)
		return 0;

	uint8_t* buf = new uint8_t[msg->FullLength];
	uint16_t* isSuccess = (uint16_t*)buf;
	*isSuccess = htons(msg->IsSuccess);

	uint16_t* errorno = (uint16_t*)(buf + 2);
	*errorno = htons(msg->ErrorNo);

	uint32_t* echoId = (uint32_t*)(buf + 4);
	*echoId = htonl(msg->EchoId);

	MsgBuf = buf;
	return buf;
	

}

uint8_t* CLCAREUpdatePKMsgSerializer::getSerializeChar()
{
	if(HeadBuf == 0 || MsgBuf == 0)
		return 0;

	uint8_t* buf = new uint8_t[FullLength + 8];
	memcpy(buf,HeadBuf,8);
	memcpy(buf+8,MsgBuf,FullLength);

	delete HeadBuf;
	delete MsgBuf;

	HeadBuf =0;
	MsgBuf = 0;

	ReStart = true;

	return buf;
}