#ifndef CLCASERIALIZER_H
#define CLCASERIALIZER_H


#include <stdint.h>
#include <vector>

class CLCAMessage;

class CLCASerializer
{
public:
	CLCASerializer();
	virtual ~CLCASerializer();

	// ����Ϣ���л�ʱ��������������Ϊ0
	//����Ϣ���л�ʱ������һΪ0
	virtual uint8_t* Serialize(CLCAMessage* message , std::vector<CLCAMessage*>* msg_vec , 
		uint32_t MsgType , uint32_t* SerializeLen ,bool IsDelete = true , bool IsHeadSerialize = true ) = 0;



};

#endif