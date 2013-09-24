#include "CLCAPostNetMsgTool.h"
#include <stdlib.h>
#include <string.h>

CLCAPostNetMsgTool::CLCAPostNetMsgTool(uint8_t IPType,uint8_t* IP,uint16_t Port)
{
	m_IPType = IPType;
	if(m_IPType == TYPEOFIPV4)
	{
		m_IP = new uint8_t[4];
		memset(m_IP,0,4);
		memcpy(m_IP,IP,4);
	}
	else if(m_IPType == TYPEOFIPV6)
	{
		m_IP = new uint8_t[16];
		memset(m_IP,0,16);
		memcpy(m_IP,IP,16);
	}
	else
		m_IP = 0;

	m_Port = Port;

}
