#include "CLCAClientPostNetMsgBySocket.h"
#include "CLCAPostNetMsgTool.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAMessage.h"
#include "CLCAGETPKMessage.h"
#include "CLCASerializer.h"
#include "CLCADeSerializer.h"
#include "CLCAClientContext.h"
#include <string.h>

#define  SERVER_IP "127.0.0.1"
#define  SERVER_PORT 8080
int main(int argc,char** argv)
{
	CLCAPostNetMsgTool* clPostTool = new CLCAClientPostNetMsgBySocket(AF_INET,(uint8_t*)SERVER_IP,SERVER_PORT);
	char* name = "TestServer";
	CLCAGETPKMessage* msg = new CLCAGETPKMessage(strlen(name)+1,(uint8_t*)name,1,0);
	CLCASerializer* serializer = new CLCAGETPKMsgSerializer;
	
	
	serializer->Serialize(msg);
	serializer->SerializeHead(PK_FORSGET,1);
	clPostTool->PostNetMessage(serializer->getSerializeChar(),serializer->getFullLength());

	delete msg;
	delete serializer;

	CLCAClientContext* context = (CLCAClientContext*)clPostTool->ReadFromNet();


	delete clPostTool;
}