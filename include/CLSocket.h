#ifndef CLSOCKET_H
#define CLSOCKET_H

#include "CLCAAddress.h"
#include <stdint.h>
#include <sys/uio.h>



class CLSocket
{
public:
	CLSocket(const uint8_t* IP,uint16_t Port,bool isneeded = false,int socketType = AF_INET,int socketStream = SOCK_STREAM);
	CLSocket(int sockfd);
	virtual ~CLSocket();


	int setNonBlock();
	int setNonBlock(int fd);
	int ConnectSocket();
	int BindSocket();
	int ListenSocket(int listenType = 0);
	int AcceptSocket();

	int WriteSocket(uint8_t* buf,uint32_t writeLen);
	int WritevSocket(struct iovec* vec,int count);
	int ReadSocket(uint32_t readLen,uint8_t* buf,uint32_t* HasReadLen);
	int SendSocket(uint8_t* buf,uint32_t sendLen);
	int ReceiveSocket(uint32_t receiveLen,uint8_t* buf,uint32_t* HasReadLen);
	int CloseSocket();

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
	bool isInput;

};
#endif