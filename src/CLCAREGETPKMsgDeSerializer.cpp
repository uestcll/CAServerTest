#include "CLCAREGETPKMsgDeSerializer.h"
#include "CLCAREGETPKMessage.h"
#include <stdlib.h>
#include <string.h>

CLCAREGETPKMsgDeSerializer::CLCAREGETPKMsgDeSerializer()
{

}

CLCAREGETPKMsgDeSerializer::~CLCAREGETPKMsgDeSerializer()
{

}

CLCAMessage* CLCAREGETPKMsgDeSerializer::DeSerializer(uint8_t* buf)
{
	uint16_t* isSuccess = (uint16_t*)buf;
	uint16_t* errorno = (uint16_t*)(buf+2);
	uint32_t* len = (uint32_t*)(buf+4);
	uint32_t* echoId = (uint32_t*)(buf+8);
	uint8_t* pk = new uint8_t[*len];
	memcpy(pk,buf+12,*len);
	CLCAMessage* msg =  new CLCAREGETPKMessage(*isSuccess,*errorno,*len,pk,*echoId);
	delete pk;
	return msg;
}