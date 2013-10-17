#ifndef CLCAREGETPKMSGSERIALIZER_H
#define CLCAREGETPKMSGSERIALIZER_H
#include "CLCASerializer.h"
#include <stdint.h>

class CLCAREGETPKMsgSerializer : public CLCASerializer
{
public:
	virtual uint8_t* Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type ,uint32_t number );
	virtual uint8_t* getSerializeChar();

	CLCAREGETPKMsgSerializer();
	virtual ~CLCAREGETPKMsgSerializer();

private:

	uint8_t* HeadBuf;
	uint8_t* MsgBuf;
	uint32_t m_nType;
	uint32_t m_number;//批量查询时，表示请求公钥的个数
	
};
#endif