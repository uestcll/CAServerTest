#include "CLCAMessage.h"

CLCAMessage::CLCAMessage(uint32_t msgID)
{
	m_MsgID = msgID;
	FullLength = 0;
}

CLCAMessage::~CLCAMessage()
{

}

