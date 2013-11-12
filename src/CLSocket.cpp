#include "CLSocket.h"
#include "CLCAAddress.h"
#include "CLCAAddressIPV4.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

CLSocket::CLSocket(const uint8_t* IP,uint16_t Port,bool isneeded,int socketType /* = AF_INET */,int socketStream /* = SOCK_STREAM */)
{
	SocketType = socketType;
	SocketStream = socketStream;
	isNeeded = isneeded;
	if(SocketType == AF_INET)
		address = new CLCAAddressIPV4(IP,Port); //���ܷ����쳣
	else
		address = new CLCAAddressIPV4(IP,Port);//Ӧ��Ϊipv6  ��ʱдΪipv4
	Initialize();//���ܷ����쳣

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

	if(!isInput)
	{
		int ret = close(sock);
		if( ret != 0)
			throw "In CLSocket::~CLSocket(), close error";
	}
}

void CLSocket::Initialize()
{
	sock = socket(SocketType,SocketStream,0);
	if(isNeeded)
		their_addr = (struct sockaddr*)malloc(sizeof(sockaddr));
	else
		their_addr = 0;
	isInput = false;
	if(sock == -1)
		throw "In CLSocket::Initialize, socket error";
}



int CLSocket::setNonBlock()
{
	int flags;
	flags = fcntl(sock, F_GETFL,0);
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
	// �쳣����-1
}

int CLSocket::BindSocket()
{
	if(isInput)
		return -1;
	return bind(sock,address->getAddr(),address->getAddrSize());
	//�쳣����-1
}

int CLSocket::ListenSocket(int listenType /* = 0 */)
{
	return listen(sock,listenType);
	//�쳣����-1
}

int CLSocket::AcceptSocket()
{
	if(isInput)
		return -1;

	socklen_t len = sizeof(sockaddr);
	return accept(sock,their_addr,&len);
	//�쳣����-1
}

int CLSocket::WriteSocket(uint8_t* buf,uint32_t writeLen)
{
	return write(sock,buf,writeLen);
}

int CLSocket::WritevSocket(struct iovec* vec,int count)
{
	return writev(sock,vec,count);
}

int CLSocket::ReadSocket(uint32_t readLen,uint8_t* buf,uint32_t* HasReadLen)
{
	int len = read(sock,buf,readLen);
	if(len >= 0)
		*HasReadLen = len; 
	else
		*HasReadLen = 0;
	return len;
}



int CLSocket::SendSocket(uint8_t* buf,uint32_t sendLen)
{
	return send(sock,buf,sendLen,0);
}



int CLSocket::ReceiveSocket(uint32_t receiveLen,uint8_t* buf,uint32_t* HasReadLen)
{
	int len = recv(sock,buf,receiveLen,0);

	if(len >= 0)
		*HasReadLen = len;
	else
		*HasReadLen = 0;
	return len;
}

int CLSocket::CloseSocket()
{
	return close(sock);
}

int CLSocket::getSock()
{
	return sock;
}