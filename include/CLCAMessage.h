#ifndef CLCAMESSAGE_H
#define CLCAMESSAGE_H

#include <stdint.h>

#define PK_FORSGET 1
#define PK_FORMGET 2
#define PK_FORRESGET 3
#define PK_FORREMGET 4
#define PK_FORSUPDATE 5
#define PK_FORRESUPDATE 6
#define PK_FORMUPDATE 7
#define PK_FORREMUPDATE 8

const int SUCCESS = 1;
const int UNSUCCESS = 0;

#define NO_ERROR 0
#define SQL_ERROR 1
#define NORECORD_ERROR 2
#define MSGNULL_ERROR 3
// 该类是所有相关认证操作的报文类，这里不包含报头。
class CLCAMessage
{


public:
	CLCAMessage(uint32_t msgID);
	virtual ~CLCAMessage();
	
	virtual CLCAMessage* copy() = 0;

private:
	CLCAMessage(const CLCAMessage&);
	CLCAMessage& operator=(const CLCAMessage&);

public:
	uint32_t m_MsgID;
	uint32_t FullLength;



//protected:
//	uint32_t m_lFullLength;

};

#endif