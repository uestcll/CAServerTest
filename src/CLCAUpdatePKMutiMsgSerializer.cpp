#include "CLCAUpdatePKMutiMsgSerializer.h"
#include <string.h>
#include <sys/socket.h>

using namespace std;

CLCAUpdatePKMutiMsgSerializer::CLCAUpdatePKMutiMsgSerializer()
{
	SingleMsgSer = new CLCAUpdatePKMsgSerializer;
	msg_list = new list<CLCAMessage*>;
	HeadBuf = 0;

}

CLCAUpdatePKMutiMsgSerializer::~CLCAUpdatePKMutiMsgSerializer()
{
	delete SingleMsgSer;
	if(HeadBuf != 0)
		delete HeadBuf;

	list<CLCAMessage*>::iterator it;
	for(it = msg_list->begin();it != msg_list->end(); it++)
	{
		if(*it != 0)
			delete *it;
	}

	delete msg_list;

}

void CLCAUpdatePKMutiMsgSerializer::SerializeHead(uint32_t Type,uint32_t number)
{
	if(HeadBuf != 0)
		delete HeadBuf;

	HeadBuf = new uint8_t[12];
	memset(HeadBuf,0,12);

	uint32_t* type = (uint32_t*)HeadBuf;
	*type = htonl(Type);

	FullLength += 4;
	uint32_t* len = (uint32_t*)(HeadBuf + 4);
	*len = htonl(FullLength);

	uint32_t* num = (uint32_t*)(HeadBuf + 8);
	*num = htonl(msg_list->size());


}

uint8_t* CLCAUpdatePKMutiMsgSerializer::Serialize(CLCAMessage* message)
{
	if(ReStart)
	{
		FullLength = 0;
		ReStart = false;
	}

	CLCAUpdatePKMessage* msg = dynamic_cast<CLCAUpdatePKMessage*>message;
	if(msg == 0)
		return 0;

	FullLength += msg->FullLength;
	msg_list->push_back(msg->copy());

	return 0;
}

uint8_t* CLCAUpdatePKMutiMsgSerializer::getSerializeChar()
{
	if(msg_list->size() == 0)
	{
		ReStart = true;
		return 0;

	}

	uint8_t* buf = new uint8_t[FullLength+12];
	memset(buf,0,FullLength+12);
	memcpy(buf,HeadBuf,12);
	CLCAMessage* msg = 0;

	uint32_t HasWriteLen = 12;
	uint8_t* tempbuf = 0;

	list<CLCAMessage*>::iterator it;
	for(it = msg_list->begin();it != msg_list->end();it++)
	{
		tempbuf = SingleMsgSer->Serialize(*it);
		memcpy(buf+HasWriteLen,tempbuf,(*it)->FullLength);

		delete tempbuf;
		delete (*it);

		tempbuf = 0;

	}

	msg_list->clear();

	delete HeadBuf ;
	HeadBuf = 0;

	ReStart = true;
	return buf;
}

