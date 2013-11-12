#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMessage.h"

#include <string.h>
#include <iostream>
#include <sys/socket.h>

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
	if(HeadBuf != 0)
		delete HeadBuf;

	HeadBuf = new uint8_t[8];
	memset(HeadBuf,0,8);
	uint32_t* type = (uint32_t*)HeadBuf;
	*type = htonl(Type);
	uint32_t* len = (uint32_t*)(HeadBuf+4);
	*len = htonl(FullLength);

}

uint8_t* CLCAGETPKMsgSerializer::Serialize(CLCAMessage* clmessage)
{
	//CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>clmessage;
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;
	}

	CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>(clmessage);
	if(message == 0)
		return 0;


	int strl = message->LengthOfName+ (message->LengthOfName%4 == 0?0:(4- message->LengthOfName%4));
	
	uint8_t* buf = new uint8_t[4+4+4+strl+1];
	memset(buf,0,4+4+4+strl+1);

	uint32_t* LengthOfName = (uint32_t*)buf;
	*LengthOfName = htonl(message->LengthOfName);

	memcpy(buf+4,message->Name,message->LengthOfName);
	
	uint32_t* pkType = (uint32_t*)(buf+4+strl);
	*pkType = htonl(message->PKType);
	
	uint32_t* echoId = (uint32_t*)(buf+4+strl+4);
	*echoId = htonl(message->EchoID);
	
	FullLength = message->FullLength;
	
	if(MsgBuf != 0)
		delete MsgBuf;

	MsgBuf = buf;
	return buf;
}




uint8_t* CLCAGETPKMsgSerializer::getSerializeChar()
{

	if(HeadBuf == 0 || MsgBuf == 0)
	{
		ReStart = true;
		return 0;
	}
	uint8_t* buf = new uint8_t[8+FullLength];
	memcpy(buf,HeadBuf,8);
	memcpy(buf+8,MsgBuf,FullLength);
	
	delete HeadBuf;
	HeadBuf = 0;
	delete MsgBuf;
	MsgBuf = 0;

	ReStart = true;
	return buf;
}

