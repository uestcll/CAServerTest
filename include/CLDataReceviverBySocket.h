#ifndef CLDATARECEVIVERBYSOCKET_H
#define CLDATARECEVIVERBYSOCKET_H
#include "CLDataReceviver.h"
#include "CLProtocolDecapsulator.h"
#include "CLCAClientContext.h"

#define  SYS_BUFSIZE 4096


class CLDataReceviverBySocket:public CLDataReceviver
{

public:
	CLDataReceviverBySocket(CLProtocolDecapsulator* ptcDcps,CLSocket* socket);
	CLDataReceviverBySocket(CLSocket* socket);
	virtual ~CLDataReceviverBySocket();

	virtual void Initialize();
	virtual void* getData();
	virtual void setContext(void* pContext);

private:

	CLSocket* m_socket;
	CLCAClientContext* con_client;

};
#endif