#include "CLCAUpdatePKMutiMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAUpdatePKMsgSerializer.h"
#include "CLCAUpdatePKMessage.h"
#include "CLLogger.h"

#include <string.h>
#include <arpa/inet.h>

using namespace std;

CLCAUpdatePKMutiMsgSerializer::CLCAUpdatePKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAUpdatePKMsgSerializer;


}

CLCAUpdatePKMutiMsgSerializer::~CLCAUpdatePKMutiMsgSerializer()
{
	delete SingleMsgSer;

}

uint8_t* CLCAUpdatePKMutiMsgSerializer::Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec ,
	uint32_t MsgType , uint32_t* SerializeLen , bool IsDelete /* = true  */,bool IsHeadSerialize /* = true */ )
{
	*SerializeLen = 0;

	if(msg_vec == 0)
		return 0;

	if(msg_vec->size() == 0)
		return 0;

	uint32_t index = 0;
	uint32_t FullLength = 0;

	vector<uint8_t*>* buf_vec = new vector<uint8_t*>;
	vector<uint32_t>* len_vec = new vector<uint32_t>;

	uint8_t* ret_buf = 0;
	uint8_t* buf = 0;

	uint32_t i = 0;
	CLCAMessage* msg = 0;

	uint32_t len = 0;

	for(i = 0;i < msg_vec->size() ; i++)
	{
		buf = 0;

		msg = msg_vec->at(i);
		buf = SingleMsgSer->Serialize(msg,0,0,&len,IsDelete,false);

		if(buf == 0)
		{
			CLLogger::WriteLogMsg("In CLCAREGETPKMutiMsgSerializer::Serialize(),singleMsgSer serialize error",0);
			continue;
		}

		FullLength += len;

		buf_vec->push_back(buf);
		len_vec->push_back(len);

	}

	if(buf_vec->size() == 0)
	{
		delete buf_vec;
		delete len_vec;

		return 0;
	}

	if(IsHeadSerialize)
	{
		FullLength += 12;

		ret_buf = new uint8_t[FullLength + 1];
		memset(ret_buf,0,FullLength +1 );

		uint32_t* HeadType = (uint32_t*)ret_buf;

		if(MsgType == 0)
			*HeadType = htonl(PK_FORMUPDATE);
		else
			*HeadType = htonl(MsgType);

		uint32_t* fullen = (uint32_t*)(ret_buf + 4);
		*fullen = htonl(FullLength - 8);

		index = 8;
	}
	else
	{
		ret_buf = new uint8_t[FullLength + 1];
		memset(ret_buf,0,FullLength + 1);

		index = 0;
	}

	uint32_t* num = (uint32_t*)(ret_buf + 8);
	*num = htonl(buf_vec->size());
	index += 4;

	for(i = 0; i< buf_vec->size() ; i++)
	{
		buf = buf_vec->at(i);
		memcpy(ret_buf + index,buf,len_vec->at(i));

		delete buf;

		index += len_vec->at(i);
	}

	*SerializeLen = FullLength;

	delete buf_vec;
	delete len_vec;

	if(IsDelete)
		delete msg_vec;

	return ret_buf;
}

