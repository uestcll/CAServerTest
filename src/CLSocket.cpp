#include "CLSocket.h"
#include "CLCAAddress.h"
#include "CLCAAddressIPV4.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

CLSocket::CLSocket(const uint8_t* IP,uint16_t Port,bool isneeded,int socketType /* = AF_INET */,int socketStream /* = SOCK_STREAM */)
{
	SocketType = socketType;
	SocketStream = socketStream;
	isNeeded = isneeded;
	if(SocketType == AF_INET)
		address = new CLCAAddressIPV4(IP,Port);
	else
		address = new CLCAAddressIPV4(IP,Port);//应该为ipv6  暂时写为ipv4
	Initialize();

}

CLSocket::CLSocket(int sockfd)
{
	sock = sockfd;
	isInput = true;
	SocketType = 0;
	SocketStream = 0;
	isNeeded = false;
	address = 0;
}

CLSocket::~CLSocket()
{
	if(address != 0)
		delete address;
	if(isNeeded)
		delete their_addr;
	close(sock);
}

void CLSocket::Initialize()
{
	sock = socket(SocketType,SocketStream,0);
	if(isNeeded)
		their_addr = (struct sockaddr*)malloc(sizeof(sockaddr));
	else
		their_addr = 0;
	isInput = false;
}



int CLSocket::setNonBlock()
{
	int flags;
	flags = fcntl(sock, F_GETFL,NULL);
	if(flags < 0)
	{
		return flags;
	}
	flags |= O_NONBLOCK;

	if(fcntl(sock, F_SETFL,flags) < 0)
		return -1;

	return 0;
}


int CLSocket::ConnectSocket()
{
	if(isInput)
		return -1;

	return connect(sock,address->getAddr(),address->getAddrSize());
}

int CLSocket::BindSocket()
{
	if(isInput)
		return -1;
	return bind(sock,address->getAddr(),address->getAddrSize());
}

int CLSocket::ListenSocket(int listenType /* = 0 */)
{
	return listen(sock,listenType);
}

int CLSocket::AcceptSocket()
{
	if(isInput)
		return -1;
	return accept(sock,their_addr,sizeof(their_addr));
}

int CLSocket::WriteSocket(uint8_t* buf,uint32_t writeLen)
{
	return write(sock,buf,writeLen);
}


uint8_t* CLSocket::ReadSocket(uint32_t readLen,uint8_t* buf,uint32_t* HasReadLen)
{
	*HasReadLen = read(sock,buf,readLen);
	return buf;
}



int CLSocket::SendSocket(uint8_t* buf,uint32_t sendLen)
{
	return send(sock,buf,sendLen,0);
}



uint8_t* CLSocket::ReceiveSocket(uint32_t receiveLen,uint8_t* buf,uint32_t* HasReadLen)
{
	*HasReadLen = receive(sock,buf,receiveLen,0);
	return buf;
}


int CLSocket::getSock()
{
	return sock;
}