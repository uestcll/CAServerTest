#include "CLCAAddress.h"
#include <sys/socket.h>

CLCAAddress::CLCAAddress(const uint8_t* IP,uint16_t Port)
{
	m_IP = IP;
	m_Port = Port;
	Initialize();
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

