#include "CLCAPostNetMsgTool.h"

CLCAPostNetMsgTool::CLCAPostNetMsgTool(uint8_t IPType,uint8_t* IP,uint16_t Port)
{
	m_IPType = IPType;
	m_IP = IP;

	m_Port = Port;

}

CLCAPostNetMsgTool::~CLCAPostNetMsgTool()
{
	
}
