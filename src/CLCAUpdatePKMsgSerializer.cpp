#include "CLCAUpdatePKMsgSerializer.h"
#include "CLCAUpdatePKMessage.h"

#include <string.h>
#include <sys/socket.h>

CLCAUpdatePKMsgSerializer::CLCAUpdatePKMsgSerializer()
{
	HeadBuf = 0;
	MsgBuf = 0;
	m_number = 0;
}

CLCAUpdatePKMsgSerializer::~CLCAUpdatePKMsgSerializer()
{
	if(HeadBuf != 0)
		delete HeadBuf;

	if(MsgBuf != 0)
		delete MsgBuf;
}
void  CLCAUpdatePKMsgSerializer::SerializeHead(uint32_t Type,uint32_t number)
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

uint8_t* CLCAUpdatePKMsgSerializer::Serialize(CLCAMessage* message)
{
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;
	}

	CLCAUpdatePKMessage* msg = dynamic_cast<CLCAUpdatePKMessage*>message;
	if(msg == 0)
		return 0;

	uint8_t* buf = new uint8_t[msg->FullLength];
	memset(buf,0,msg->FullLength);

	uint32_t* echoid = (uint32_t*)buf;
	*echoid = htonl(msg->EchoId);

	uint32_t* lenForPW = (uint32_t*)(buf + 4);
	*lenForPW = htonl(msg->LenForPW);

	uint32_t* lenForPWAW = (uint32_t*)(buf + 8);
	*lenForPWAW = htonl(msg->LenForPKAW);

	uint32_t* pktype = (uint32_t*)(buf + 12);
	*pktype = htonl(msg->PKType);

	uint32_t* pklen = (uint32_t*)(buf + 16);
	*pklen = htonl(msg->PKLen);

	memcpy(buf+20,msg->PublicWord,msg->LenForPW);
	uint32_t ReservedLen1 = (msg->LenForPW % 4 == 0 ? 0 : (4 - msg->LenForPW % 4));

	memcpy(buf+20+msg->LenForPW+ReservedLen1,msg->Passwd_PKAW,msg->LenForPKAW);

	FullLength  = msg->FullLength;

	if(MsgBuf != 0)
		delete MsgBuf;

	MsgBuf = buf;
	return buf;
}

uint8_t* CLCAUpdatePKMsgSerializer::getSerializeChar()
{

	if(HeadBuf == 0 || MsgBuf == 0)
		return 0;

	uint8_t* buf = new uint8_t[FullLength+8];
	memcpy(buf,HeadBuf,8);
	memcpy(buf+8,MsgBuf,FullLength);

	delete HeadBuf;
	delete MsgBuf;

	HeadBuf = 0;
	MsgBuf = 0;

	ReStart = true;
	return buf;
}