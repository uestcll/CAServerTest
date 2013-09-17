#ifndef CLSOCKET_H
#define CLSOCKET_H

#include "CLCAAddress.h"
#include <stdint.h>




class CLSocket
{
public:
	CLSocket(uint8_t* IP,uint16_t Port,bool isneeded = false,int socketType = AF_INET,int socketStream = SOCK_STREAM);
	virtual ~CLSocket();


	int setNonBlock();
	int setNonBlock(int fd);
	int ConnectSocket();
	int BindSocket();
	int ListenSocket(int listenType = 0);
	int AcceptSocket();
	int WriteSocket(uint8_t* buf,uint32_t writeLen);
	int WriteSocket(uint8_t* buf,uint32_t writeLen,int writeSock);
	uint8_t* ReadSocket(uint32_t readLen,uint8_t* buf,uint32_t* HasReadLen);
	uint8_t* ReadSocket(uint32_t readLen,uint8_t* buf,uint32_t* HasReadLen,int readSock);
	int SendSocket(uint8_t* buf,uint32_t sendLen);
	int SendSocket(uint8_t* buf,uint32_t sendLen,int sendSock);
	uint8_t* ReceiveSocket(uint32_t receiveLen,uint8_t* buf,uint32_t* HasReadLen);
	uint8_t* ReceiveSocket(uint32_t receiveLen,uint8_t* buf,uint32_t* HasReadLen,int receiveSock);
	int getSock();
	
private:
	void Initialize();

private:
	int sock;
	CLCAAddress* address;
	int SocketType;
	int SocketStream;
	struct sockaddr* their_addr;
	bool isNeeded;

};
#endif