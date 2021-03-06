#ifndef CLDATARECEVIVERBYSOCKET_H
#define CLDATARECEVIVERBYSOCKET_H

#include "CLDataReceviver.h"

#include <stdint.h>

#define  SYS_BUFSIZE 4096

class CLSocket;

class CLDataReceviverBySocket:public CLDataReceviver
{

public:
	
	CLDataReceviverBySocket();
	virtual ~CLDataReceviverBySocket();

	virtual void Initialize();
	virtual void* getData(uint32_t readSize = 0xffffffff);
	virtual void setContext(void* pContext);

private:
	uint32_t m_ReadSize;
	CLSocket* m_socket;

};
#endif