#ifndef CLCAUPDATEPKMESSAGE_H
#define CLCAUPDATEPKMESSAGE_H

#include "CLCAMessage.h"

#include <stdint.h>

class CLCAUpdatePKMessage : public CLCAMessage
{
public:
	uint32_t EchoId;
	uint32_t LenForServerName;
	uint32_t LenForPW;
	uint32_t LenForPKAW;
	uint32_t PKType;
	uint32_t PKLen;
	uint8_t* ServerName;
	uint8_t* PublicWord;
	uint8_t* Passwd_PKAW;

public:
	
	CLCAUpdatePKMessage(uint32_t echoId,uint32_t lenForServerName,uint32_t lenForPW,
		uint32_t lenForPKAW,uint32_t pkType,uint32_t pklen,uint8_t* serverName,
		uint8_t* publicword,uint8_t* passwd_PKAW);

	virtual ~CLCAUpdatePKMessage();

	virtual CLCAMessage* copy();
};

#endif