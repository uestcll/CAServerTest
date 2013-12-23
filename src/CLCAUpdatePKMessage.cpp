#include "CLCAUpdatePKMessage.h"

#include <string.h>

CLCAUpdatePKMessage::CLCAUpdatePKMessage(uint32_t echoId,uint32_t lenForServerName,uint32_t lenForPW, uint32_t lenForPKAW,
	uint32_t pkType,uint32_t pklen,uint8_t* serverName, uint8_t* publicword,uint8_t* passwd_PKAW):CLCAMessage(PK_FORSUPDATE)
{
	EchoId = echoId;
	LenForServerName = lenForServerName;
	LenForPW = lenForPW;
	LenForPKAW = lenForPKAW;
	PKType = pkType;
	PKLen = pklen;

	ServerName = new uint8_t[LenForServerName + 1];
	memset(ServerName,0,LenForServerName + 1);
	memcpy(ServerName,serverName,LenForServerName);

	PublicWord = new uint8_t[LenForPW + 1];
	memset(PublicWord,0,LenForPW + 1);
	memcpy(PublicWord,publicword,LenForPW);


	Passwd_PKAW = new uint8_t[LenForPKAW + 1];
	memset(Passwd_PKAW,0,LenForPKAW + 1);
	memcpy(Passwd_PKAW,passwd_PKAW,LenForPKAW);


//	FullLength = 20 + LenForPW  + LenForPKAW  ;

}

CLCAUpdatePKMessage::~CLCAUpdatePKMessage()
{
	if(PublicWord != 0)
		delete PublicWord;

	if(Passwd_PKAW != 0)
		delete Passwd_PKAW;
}

CLCAMessage* CLCAUpdatePKMessage::copy()
{
	return new CLCAUpdatePKMessage(EchoId,LenForServerName,LenForPW,LenForPKAW,PKType,PKLen,ServerName,PublicWord,Passwd_PKAW);
}