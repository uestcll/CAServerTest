#include "CLCASerializer.h"
CLCASerializer::CLCASerializer()
{
	FullLength = 0;
}

CLCASerializer::~CLCASerializer()
{

}

uint32_t CLCASerializer::getLength()
{
	return FullLength;
}