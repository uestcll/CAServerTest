#include "HeadFile.h"
#include <string.h>
#include <iostream>

using namespace std;

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
	
	CLCAREGETPKMsgDeSerializer* deser = new CLCAREGETPKMsgDeSerializer;
	CLCAREGETPKMessage* remsg = (CLCAREGETPKMessage*)deser->DeSerializer(context->buf);
	cout<<"responsed msg is:"<<endl;
	cout<<"isSuccess:"<<remsg->isSuccess<<endl;
	cout<<"errorno:"<<remsg->errorno<<endl;
	cout<<"pklen:"<<remsg->len<<endl;
	cout<<"echoId:"<<remsg->EchoId<<endl;
	delete deser;
	delete remsg;
	delete context;
	delete clPostTool;
}