#include "CLCAREGETPKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAREGETPKMessage.h"

#include <string.h>
#include <iostream>
#include <arpa/inet.h>

CLCAREGETPKMsgSerializer::CLCAREGETPKMsgSerializer()
{

}

CLCAREGETPKMsgSerializer::~CLCAREGETPKMsgSerializer()
{

}


uint8_t* CLCAREGETPKMsgSerializer::Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec ,
	uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete /* = true  */,bool IsHeadSerialize /* = true */ )
{
	*SerializeLen = 0;

	if(message == 0 )
		return 0;

	CLCAREGETPKMessage* msg = (CLCAREGETPKMessage*)message;

	if(msg == 0)
		return 0;

	uint32_t index = 0;
	uint32_t FullLength = 0;
	uint8_t* buf = 0;

	uint32_t reserveLen = (msg->len % 4 == 0 ? 0 : (4 - msg->len % 4));
	
	FullLength = 8 + msg->len + 4 + reserveLen;

	if(IsHeadSerialize)
	{
		FullLength += 8;
		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		uint32_t* HeadType = (uint32_t*)buf;

		if(MsgType == 0)
			*HeadType = htonl(PK_FORRESGET);
		else
			*HeadType = htonl(MsgType);

		uint32_t* fullen = (uint32_t*)(buf + 4);
		*fullen = htonl(FullLength - 8);

		index = 8;

	}
	else
	{
		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		index = 0;
	}
	
	uint16_t* isSuccess = (uint16_t*)(buf + index);
	*isSuccess = htons(msg->isSuccess);
	index += 2;

	uint16_t* errorno = (uint16_t*)(buf + index);
	*errorno = htons(msg->errorno);
	index += 2;

	uint32_t* len  = (uint32_t*)(buf + index);
	*len = htonl(msg->len);
	index += 4;

	uint32_t* echoid = (uint32_t*)(buf + index);
	*echoid = htonl(msg->EchoId);
	index += 4;

	memcpy(buf + index,msg->PublicKey,msg->len);

	*SerializeLen = FullLength;

	if(IsDelete)
		delete message;

	return buf;
}
