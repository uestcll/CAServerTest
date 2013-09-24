#include "CLCAGETPKMutiMsgSerializer.h"
#include <string.h>

CLCAGETPKMutiMsgSerializer::CLCAGETPKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAGETPKMsgSerializer;
	strMsg.clear();
}

CLCAGETPKMutiMsgSerializer::~CLCAGETPKMutiMsgSerializer()
{
	delete SingleMsgSer;
	strMsg.clear();
}

void CLCAGETPKMutiMsgSerializer::SerializeHead(uint32_t Type /* = PK_FORMGET */,uint32_t number)
{
	MsgType = Type;
	MsgNum = number;
	uint8_t* Head = new uint8_t[12];
	memset(Head,0,12);
	uint32_t* type = (uint32_t*)Head;
	uint32_t* num = (uint32_t*)(Head+8);
	*type = MsgType;
	*num = MsgNum;
	strMsg.append((char*)Head,12);
	FullLength += 4;
	delete Head;
}

void CLCAGETPKMutiMsgSerializer::Serialize(CLCAMessage* message)
{

		SingleMsgSer->clearString();
		SingleMsgSer->Serialize(message);
		strMsg += SingleMsgSer->getBufString();
		FullLength += SingleMsgSer->getLength();
}

uint8_t* CLCAGETPKMutiMsgSerializer::getSerializeChar()
{
	return (uint8_t*)strMsg.data();
}

void CLCAGETPKMutiMsgSerializer::SerializeLength()
{
	uint8_t* len = new uint8_t[4];
	uint32_t* Len = (uint32_t*)len;
	*Len = FullLength;
	strMsg.replace(4,4,(char*)len);
	delete len;
}