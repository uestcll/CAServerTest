#include "CLCAGETPKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include <string.h>
#include <iostream>


CLCAGETPKMsgSerializer::CLCAGETPKMsgSerializer()
{
	MsgBuf = 0;
	HeadBuf = 0;
}

CLCAGETPKMsgSerializer::~CLCAGETPKMsgSerializer()
{
	if(MsgBuf != 0)
		delete MsgBuf;

	if(HeadBuf != 0)
		delete HeadBuf;
}

void CLCAGETPKMsgSerializer::SerializeHead(uint32_t Type /* = PK_FORSGET */,uint32_t number /* = 1 */)
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

uint8_t* CLCAGETPKMsgSerializer::Serialize(CLCAMessage* clmessage)
{
	//CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>clmessage;
	CLCAGETPKMessage* message = (CLCAGETPKMessage*)clmessage;
	if(message == 0)
		return 0;
	int strl = message->LengthOfName+ (message->LengthOfName%4 == 0?0:(4- message->LengthOfName%4));
	
	uint8_t* buf = new uint8_t[4+4+4+strl+1];
	memset(buf,0,4+4+4+strl+1);
	uint32_t* LengthOfName = (uint32_t*)buf;
	*LengthOfName = message->LengthOfName;
	memcpy(buf+4,message->Name,message->LengthOfName);
	uint32_t* pkType = (uint32_t*)(buf+4+strl);
	*pkType = message->PKType;
	uint32_t* echoId = (uint32_t*)(buf+4+strl+4);
	*echoId = message->EchoID;
	FullLength = message->FullLength;
	
	MsgBuf = buf;

	return buf;
}




uint8_t* CLCAGETPKMsgSerializer::getSerializeChar()
{
	uint8_t* buf = new uint8_t[8+FullLength];
	memcpy(buf,HeadBuf,8);
	memcpy(buf+8,MsgBuf,FullLength);
	return buf;
}

