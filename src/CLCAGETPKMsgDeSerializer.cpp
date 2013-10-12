#include "CLCAGETPKMsgDeSerializer.h"
#include "CLCAGETPKMessage.h"
#include <stdlib.h>
#include <string.h>

CLCAGETPKMsgDeSerializer::CLCAGETPKMsgDeSerializer()
{

}

CLCAGETPKMsgDeSerializer::~CLCAGETPKMsgDeSerializer()
{

}

CLCAMessage* CLCAGETPKMsgDeSerializer::DeSerializer(uint8_t* buf)
{
	uint32_t* LengthOfName = (uint32_t*)buf;
	uint8_t* Name = new uint8_t(*LengthOfName);
	memcpy(Name,buf+4,*LengthOfName);
	int strl = *LengthOfName+ ((*LengthOfName)%4 == 0?0:(4- (*LengthOfName%4)));
	uint32_t* pkType = (uint32_t*)(buf+4+strl);
	uint32_t* echoId = (uint32_t*)(buf+4+strl+4);
	CLCAGETPKMessage* msg = new CLCAGETPKMessage(*LengthOfName,Name,*pkType,*echoId);
	delete Name;
	return msg;
}