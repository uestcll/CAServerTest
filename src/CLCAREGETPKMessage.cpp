#include "CLCAREGETPKMessage.h"

#include <string.h>

CLCAREGETPKMessage::CLCAREGETPKMessage(uint16_t IsSuccess,uint16_t errorNo,uint32_t Len,uint8_t* pk,uint32_t echoId):CLCAMessage(PK_FORRESGET)
{
	isSuccess = IsSuccess;
	errorno = errorNo;
	len = Len;

	PublicKey = new uint8_t[len + 1];
	memset(PublicKey,0,len + 1);
	memcpy(PublicKey,pk,len);

	EchoId = echoId;

//	FullLength = 8+len + 4;

}

CLCAREGETPKMessage::~CLCAREGETPKMessage()
{

}

CLCAMessage* CLCAREGETPKMessage::copy()
{
	return new CLCAREGETPKMessage(isSuccess,errorno,len,PublicKey,EchoId);
}