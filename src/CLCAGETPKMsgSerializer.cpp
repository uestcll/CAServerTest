#include "CLCAGETPKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include <string.h>
#include <iostream>

using namespace std;

CLCAGETPKMsgSerializer::CLCAGETPKMsgSerializer()
{
	strmessage.clear();
}

CLCAGETPKMsgSerializer::~CLCAGETPKMsgSerializer()
{

}

void CLCAGETPKMsgSerializer::SerializeHead(uint32_t Type /* = PK_FORSGET */,uint32_t number /* = 1 */)
{
	m_nType = Type;
	m_number = number;
	uint8_t* Head = new uint8_t[8];
	memset(Head,0,8);
	uint32_t* type = (uint32_t*)Head;
	*type = Type;
	strmessage.append((char*)Head,8);
	delete Head;

}

void CLCAGETPKMsgSerializer::Serialize(CLCAMessage* clmessage)
{
	//CLCAGETPKMessage* message = dynamic_cast<CLCAGETPKMessage*>clmessage;
	CLCAGETPKMessage* message = (CLCAGETPKMessage*)clmessage;
	if(message == 0)
		return ;
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
	FullLength += message->FullLength;
	strmessage.append((char*)buf,message->FullLength);
	delete buf;

	
}

string CLCAGETPKMsgSerializer::getBufString()
{
	return strmessage;
}

void CLCAGETPKMsgSerializer::clearString()
{
	strmessage.clear();
	FullLength = 0;
}

uint8_t* CLCAGETPKMsgSerializer::getSerializeChar()
{
	return (uint8_t*)strmessage.c_str();
}

void CLCAGETPKMsgSerializer::SerializeLength()
{
	uint8_t* len = new uint8_t[4];
	uint32_t* Len = (uint32_t*)len;
	*Len = FullLength;
	strmessage.replace(4,4,(char*)len);
	delete len;
}
