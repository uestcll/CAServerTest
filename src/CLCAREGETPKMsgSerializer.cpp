#include "CLCAREGETPKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAREGETPKMessage.h"
#include <string.h>
#include <iostream>


CLCAREGETPKMsgSerializer::CLCAREGETPKMsgSerializer()
{
	MsgBuf = 0;
	HeadBuf = 0;
}

CLCAREGETPKMsgSerializer::~CLCAREGETPKMsgSerializer()
{
	if(MsgBuf != 0)
		delete MsgBuf;

	if(HeadBuf != 0)
		delete HeadBuf;
}

CLCAREGETPKMsgSerializer::SerializeHead(uint32_t Type ,uint32_t number )
{
	m_nType = Type;
	m_number = number;
	HeadBuf = new uint8_t[8];
	memset(HeadBuf,0,8);
	uint32_t* type = (uint32_t*)HeadBuf;
	*type = Type;
	uint32_t* len = (uint32_t*)(HeadBuf+4);
	*len = FullLength;
}

CLCAREGETPKMsgSerializer::Serialize(CLCAMessage* message)
{
	CLCAREGETPKMessage* msg = (CLCAREGETPKMessage*)message;
	if(msg == 0)
		return 0;
	uint8_t* buf = new uint8_t[msg->FullLength+1];
	memset(buf,0,msg->FullLength+1);

	uint16_t* isSuccess = (uint16_t*)buf;
	*isSuccess = msg->isSuccess;
	uint16_t* errorno = (uint16_t*)(buf+2);
	*errorno = msg->errorno;
	uint32_t* len  = (uint32_t*)(buf+4);
	*len = msg->len;
	uint32_t* echoid = (uint32_t*)(buf+8);
	*echoid = msg->EchoId;
	memcpy(buf+12,msg->PublicKey,msg->len);
	FullLength = msg->FullLength;

	MsgBuf = buf;
	return buf;
}

uint8_t* CLCAREGETPKMsgSerializer::getSerializeChar()
{
	uint8_t* buf = new uint8_t[8+FullLength];
	memcpy(buf,HeadBuf,8);
	memcpy(buf+8,MsgBuf,FullLength);
	return buf;
}