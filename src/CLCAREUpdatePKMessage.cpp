#include "CLCAREUpdatePKMessage.h"


CLCAREUpdatePKMessage::CLCAREUpdatePKMessage(uint16_t isSuccess,uint16_t errorNo,uint32_t echoId):CLCAMessage(PK_FORRESUPDATE)
{
	IsSuccess = isSuccess;
	ErrorNo = errorNo;
	EchoId = echoId;

//	FullLength = 8;
}

CLCAREUpdatePKMessage::~CLCAREUpdatePKMessage()
{

}

CLCAMessage* CLCAREUpdatePKMessage::copy()
{
	return new CLCAREUpdatePKMessage(IsSuccess,ErrorNo,EchoId);
}