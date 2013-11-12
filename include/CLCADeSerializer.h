#ifndef CLCADESERIALIZER_H
#define CLCADESERIALIZER_H

#include "CLCAMessage.h"
#include <stdint.h>
#include <vector>

class CLBuffer;
// 这是CLCAMessage的反初始化操作
class CLCADeSerializer
{
public:
	virtual std::vector<CLCAMessage*>* DeSerializer(CLBuffer* Buf,uint32_t startindex) = 0;
	CLCADeSerializer();
	virtual ~CLCADeSerializer();
};
#endif