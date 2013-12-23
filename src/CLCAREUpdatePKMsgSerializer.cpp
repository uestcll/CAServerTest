#include "CLCAREUpdatePKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAREUpdatePKMessage.h"

#include <arpa/inet.h>
#include <string.h>

CLCAREUpdatePKMsgSerializer::CLCAREUpdatePKMsgSerializer()
{

}

CLCAREUpdatePKMsgSerializer::~CLCAREUpdatePKMsgSerializer()
{

}


uint8_t* CLCAREUpdatePKMsgSerializer::Serialize(CLCAMessage* message, std::vector<CLCAMessage*>* msg_vec ,
	uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete /* = true  */,bool IsHeadSerialize /* = true */ )
{
	*SerializeLen = 0;

	if(message == 0)
		return 0;

	CLCAREUpdatePKMessage* msg = dynamic_cast<CLCAREUpdatePKMessage*>(message);
	
	if(msg == 0)
		return 0;

	uint8_t* buf = 0;
	uint32_t FullLength = 0;
	uint32_t index = 0;

	FullLength = 8;

	if(IsHeadSerialize)
	{
		FullLength += 8;
		buf = new uint8_t[FullLength + 1];
		
		memset(buf,0,FullLength + 1);

		uint32_t* HeadType = (uint32_t*)buf;

		if(MsgType == 0)
			*HeadType = htonl(PK_FORRESUPDATE);
		else
			*HeadType = htonl(MsgType);

		uint32_t* len = (uint32_t*)(buf + 4);
		*len = htonl(FullLength - 8);

		index = 8;
	}
	else
	{
		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		index = 0;
	}

	uint16_t* isSuccess = (uint16_t*)(buf + index);
	*isSuccess = htons(msg->IsSuccess);
	index += 2;

	uint16_t* errorno = (uint16_t*)(buf + index);
	*errorno = htons(msg->ErrorNo);
	index += 2;

	uint32_t* echoId = (uint32_t*)(buf + index);
	*echoId = htonl(msg->EchoId);

	*SerializeLen = FullLength;

	if(IsDelete)
		delete message;

	return buf;
	

}

