#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMessage.h"
#include "CLCAMessage.h"

#include <string.h>
#include <iostream>
#include <arpa/inet.h>

CLCAGETPKMsgSerializer::CLCAGETPKMsgSerializer()
{
	
}

CLCAGETPKMsgSerializer::~CLCAGETPKMsgSerializer()
{
	
}

uint8_t* CLCAGETPKMsgSerializer::Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec ,
	uint32_t MsgType , uint32_t* SerializeLen ,bool IsDelete /* = true  */,bool IsHeadSerialize /* = true */ )
{
	*SerializeLen = 0;

	if( message == 0 )
		return 0;

	CLCAGETPKMessage* msg = dynamic_cast<CLCAGETPKMessage*>(message);

	if(message == 0)
		return 0;

	uint32_t index = 0;
	uint8_t* buf = 0;

	uint32_t reserveLen = (msg->LengthOfName % 4 == 0 ? 0 : (4 - msg->LengthOfName%4));
	uint32_t FullLength = 0;

	if(IsHeadSerialize)
	{
		FullLength = 12 + msg->LengthOfName + reserveLen + 8;
		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		uint32_t* HeadType = (uint32_t*)buf;

		if(MsgType == 0)
			*HeadType = htonl(PK_FORSGET);
		else
			*HeadType = htonl(MsgType);

		uint32_t* len = (uint32_t*)(buf + 4);
		*len = htonl(FullLength - 8);

		index = 8;
	}
	else
	{
		FullLength = 4 + msg->LengthOfName + reserveLen + 8;
		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		index = 0;
	}

	*SerializeLen = FullLength;

	uint32_t* lengthofName = (uint32_t*)(buf + index);
	*lengthofName = htonl(msg->LengthOfName);

	index += 4;

	memcpy(buf + index , msg->Name , msg->LengthOfName);
	index += msg->LengthOfName;

	index += reserveLen;

	uint32_t* pktype = (uint32_t*)(buf + index);
	*pktype = htonl(msg->PKType);
	index += 4;

	uint32_t* echoId = (uint32_t*)(buf + index);
	*echoId = htonl(msg->EchoID);
	index += 4;

	if(IsDelete)
		delete message;

	return buf;
}