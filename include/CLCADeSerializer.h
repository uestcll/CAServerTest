#ifndef CLCADESERIALIZER_H
#define CLCADESERIALIZER_H
#include "CLCAMessage.h"
#include <stdint.h>
// ����CLCAMessage�ķ���ʼ������
class CLCADeSerializer
{
public:
	virtual CLCAMessage* DeSerializer(uint8_t* buf) = 0;
	CLCADeSerializer();
	virtual ~CLCADeSerializer();
};
#endif