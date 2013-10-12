#include "CLCAREGETPKMessage.h"
#include <string.h>

CLCAREGETPKMessage::CLCAREGETPKMessage(uint16_t IsSuccess,uint16_t errorNo,uint32_t Len,uint8_t* pk,uint32_t echoId):CLCAMessage(PK_FORRESGET)
{
	isSuccess = IsSuccess;
	errorno = errorNo;
	len = Len;
	PublicKey = new uint8_t[len];
	memset(PublicKey,0,len);
	memcpy(PublicKey,pk,len);
	Reserved = 0;
	EchoId = echoId;
	int ReservedLen = len%4 == 0?0:(4-len%4);

	FullLength = 8+len+4+ReservedLen;

}

CLCAREGETPKMessage::~CLCAREGETPKMessage()
{

}

CLCAMessage* CLCAREGETPKMessage::copy()
{
	return new CLCAREGETPKMessage(isSuccess,errorno,len,PublicKey,EchoId);
}