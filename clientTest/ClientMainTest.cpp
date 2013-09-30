#include "CLCAClientPostNetMsgBySocket.h"
#include "CLCAPostNetMsgTool.h"

#define  SERVER_IP "192.168.0.1"
#define  SERVER_PORT 19999
int main(int argc,char** argv)
{
	CLCAPostNetMsgTool* clPostTool = new CLCAClientPostNetMsgBySocket(AF_INET,(uint8_t*)SERVER_IP,SERVER_PORT);

	delete clPostTool;
}