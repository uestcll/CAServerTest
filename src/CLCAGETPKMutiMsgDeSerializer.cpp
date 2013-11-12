#include "CLCAGETPKMutiMsgDeSerializer.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCAGETPKMsgDeSerializer.h"
#include "CLLogger.h"
#include <sys/socket.h>

using namespace std;

CLCAGETPKMutiMsgDeSerializer::CLCAGETPKMutiMsgDeSerializer()
{
	Msg_number = 0;
	SingleDeSer = new CLCAGETPKMsgDeSerializer;
}

CLCAGETPKMutiMsgDeSerializer::~CLCAGETPKMutiMsgDeSerializer()
{
	delete SingleDeSer;
}

void CLCAGETPKMutiMsgDeSerializer::getMsgNumber(uint8_t* buf)
{
	uint32_t* num = (uint32_t*)buf;
	Msg_number = ntohl(*num);

}


vector<CLCAMessage*>* CLCAGETPKMutiMsgDeSerializer::DeSerializer(CLBuffer* Buf,uint32_t startindex)
{
	if(-1 == Buf->setSuccessionIndex(startindex))
	{
		CLLogger::WriteLog("In CLCAGETPKMutiMsgDeSerializer::DeSerializer(),Buf setSuccessionIndex error"0);
		return 0;
	}
	uint8_t* buf = 0;
	bool IsDeleted = false;
	buf = Buf->getSuccessionBuf(4,&IsDeleted);
	if(buf == 0)
	{
		CLLogger::WriteLog("In CLCAGETPKMutiMsgDeSerializer::DeSerializer(),Buf getSuccessionBuf",0);
		return 0;
	}

	getMsgNumber(buf);
	if(IsDeleted)
		delete buf;

	int NextIndex = 0;
	vector<CLCAMessage*>* ret_vec = new vector<CLCAMessage*>;
	vector<CLCAMessage*>* single_vec = 0;
	
	CLCAMessage* msg = 0;

	for(int i = 0;i<Msg_number;i++)
	{
		NextIndex = Buf->getSuccessionIndex();
		single_vec = SingleDeSer->DeSerializer(Buf,NextIndex);
		if(single_vec == 0) //一旦出错 则返回0
		{
			CLLogger::WriteLog("In CLCAGETPKMutiMsgDeSerializer::DeSerializer(),SingleDeSer DeSerializer() error",0);
			for(int j = 0; j< ret_vec->size(); j++)
			{
				msg = ret_vec->at(j);
				if(msg != 0)
					delete msg;
			}

			delete ret_vec;
			return 0;
		}

		ret_vec->push_back(single_vec->at(0));
		delete single_vec;
	}

	if(ret_vec->size() == 0)
	{
		delete ret_vec;
		return 0;
	}

	return ret_vec;
}


