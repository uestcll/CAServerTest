#ifndef CLCADESERIALIZER_H
#define CLCADESERIALIZER_H
#include "CLCAMessage.h"
#include <stdint.h>
// 这是CLCAMessage的反初始化操作
class CLCADeSerializer
{
public:
	virtual CLCAMessage* DeSerializer(uint8_t* buf) = 0;
	CLCADeSerializer();
	virtual ~CLCADeSerializer();
};
#endif