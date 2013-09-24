#ifndef CLCAGETPKMSGSERIALIZER_H
#define CLCAGETPKMSGSERIALIZER_H
#include <iostream>
#include <stdint.h>
#include "CLCAMessage.h"
#include "CLCASerializer.h"
//���Ƕ�CLCAGETMessage������л��������������紫��
class CLCAGETPKMsgSerializer : public CLCASerializer
{
public:
	virtual void Serialize(CLCAMessage* message);
	virtual void SerializeHead(uint32_t Type ,uint32_t number );
	virtual void SerializeLength();
	virtual uint8_t* getSerializeChar();
	
	std::string getBufString();
	void clearString();
	CLCAGETPKMsgSerializer();
	virtual ~CLCAGETPKMsgSerializer();

private:
	std::string strmessage;
	uint32_t m_nType;
	uint32_t m_number;//������ѯʱ����ʾ����Կ�ĸ���

};
#endif