#include "CLCAUpdatePKMsgSerializer.h"
#include "CLCAUpdatePKMessage.h"
#include "CLCAMessage.h"

#include <string.h>
#include <arpa/inet.h>

CLCAUpdatePKMsgSerializer::CLCAUpdatePKMsgSerializer()
{
	
}

CLCAUpdatePKMsgSerializer::~CLCAUpdatePKMsgSerializer()
{
	
}

uint8_t* CLCAUpdatePKMsgSerializer::Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec ,
	uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete /* = true  */,bool IsHeadSerialize /* = true */ )
{
	*SerializeLen = 0;

	if(message == 0)
		return 0;


	CLCAUpdatePKMessage* msg = dynamic_cast<CLCAUpdatePKMessage*>(message);
	
	if(msg == 0)
		return 0;

	uint32_t FullLength = 0;
	uint8_t* buf = 0;
	uint32_t index = 0;

	uint32_t ReservedLen1 = (msg->LenForServerName % 4 == 0 ? 0 :(4 - msg->LenForServerName % 4));
	uint32_t ReservedLen2 = (msg->LenForPW % 4 == 0 ? 0 : (4 - msg->LenForPW % 4));
	uint32_t ReservedLen3 = (msg->LenForPKAW % 4 == 0 ? 0 : (4 - msg->LenForPKAW % 4));

	FullLength = ReservedLen1 + ReservedLen2 + ReservedLen3 + 24 + msg->LenForServerName + msg->LenForPKAW + msg->LenForPW;

	if(IsHeadSerialize)
	{
		FullLength += 8;

		buf = new uint8_t[FullLength + 1];
		memset(buf,0,FullLength + 1);

		uint32_t* HeadType = (uint32_t*)buf;

		if(MsgType == 0)
			*HeadType = htonl(PK_FORSUPDATE);
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

	uint32_t* echoid = (uint32_t*)(buf + index);
	*echoid = htonl(msg->EchoId);
	index += 4;

	uint32_t* lenForServerName = (uint32_t*)(buf + index);
	*lenForServerName = htonl(msg->LenForServerName);
	index += 4;

	uint32_t* lenForPW = (uint32_t*)(buf + index);
	*lenForPW = htonl(msg->LenForPW);
	index += 4;

	uint32_t* lenForPWAW = (uint32_t*)(buf + index);
	*lenForPWAW = htonl(msg->LenForPKAW);
	index += 4;

	uint32_t* pktype = (uint32_t*)(buf + index);
	*pktype = htonl(msg->PKType);
	index += 4;

	uint32_t* pklen = (uint32_t*)(buf + index);
	*pklen = htonl(msg->PKLen);
	index += 4;

	memcpy(buf + index,msg->ServerName,msg->LenForServerName);
	index += msg->LenForServerName;
	index += ReservedLen1;

	memcpy(buf + index,msg->PublicWord,msg->LenForPW);
	index += msg->LenForPW;
	index += ReservedLen2;
	

	memcpy(buf + index,msg->Passwd_PKAW,msg->LenForPKAW);

	*SerializeLen = FullLength;

	if(IsDelete)
		delete message;

	return buf;
}

