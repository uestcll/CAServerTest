#include "CLCAAddress.h"


CLCAAddress::CLCAAddress(const uint8_t* IP,uint16_t Port)
{
	m_IP = IP;
	m_Port = Port;
	
}

CLCAAddress::~CLCAAddress()
{

}

//int CLCAAddress::getSock()
//{
//	return sock;
//}


struct sockaddr* CLCAAddress:: getAddr()
{
	return addr;
}

