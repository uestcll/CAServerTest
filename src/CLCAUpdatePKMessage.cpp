#include "CLCAUpdatePKMessage.h"

#include <string.h>

CLCAUpdatePKMessage::CLCAUpdatePKMessage(uint32_t echoId,uint32_t l lenForPW,uint32_t lenForPKAW,
	uint32_t pkType,uint32_t pklen,uint8_t* publicword, uint8_t* passwd_PKAW):CLCAMessage(PK_FORSUPDATE)
{
	EchoId = echoId;
	LenForPW = lenForPW;
	LenForPKAW = lenForPKAW;
	PKType = pkType;
	PKLen = pklen;

	PublicWord = new uint8_t[LenForPW];
	memcpy(PublicWord,publicword,LenForPW);

	uint32_t ReservedLen1 = (LenForPW % 4 == 0 ? 0 : (4 - LenForPW % 4));

	Passwd_PKAW = new uint8_t[LenForPKAW];
	memcpy(Passwd_PKAW,passwd_PKAW,LenForPKAW);

	uint32_t ReservedLen2 = (LenForPKAW % 4 == 0 ? 0 : (4 - LenForPKAW % 4));

	FullLength = 20 + LenForPW + ReservedLen1 + LenForPKAW + ReservedLen2;

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
	return new CLCAUpdatePKMessage(EchoId,LenForPW,LenForPKAW,PKType,PKLen,PublicWord,Passwd_PKAW);
}