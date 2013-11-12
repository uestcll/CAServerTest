#include "CLCASerializer.h"

CLCASerializer::CLCASerializer()
{
	FullLength = 0;
	ReStart = false;
}

CLCASerializer::~CLCASerializer()
{

}

uint32_t CLCASerializer::getLength()
{
	return FullLength;
}

uint32_t CLCASerializer::getFullLength()
{
	return FullLength+8;
}