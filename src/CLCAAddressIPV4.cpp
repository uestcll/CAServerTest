#include "CLCAAddressIPV4.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

CLCAAddressIPV4::CLCAAddressIPV4(const uint8_t* IP,const uint16_t Port):CLCAAddress(IP,Port)
{

}

void CLCAAddressIPV4::Initialize()
{
//	sock = socket(AF_INET,SOCK_STREAM,0);
	addr_ipv4 = (sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	memset((char*)addr_ipv4,0,sizeof(addr_ipv4));
	addr_ipv4->sin_family = AF_INET;
	addr_ipv4->sin_port = htons(m_Port);
	inet_pton(AF_INET,(char*)m_IP,addr_ipv4->sin_addr);
	addr = (struct sockaddr*)addr_ipv4;
}

int CLCAAddressIPV4::getAddrSize()
{
	return sizeof(addr_ipv4);
}