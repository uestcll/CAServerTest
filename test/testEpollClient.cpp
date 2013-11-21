#include "CLSocket.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

using namespace std;

#define PORT 50020

int main()
{
	CLSocket* sock = new CLSocket("127.0.0.1",PORT,true);
	int r = sock->ConnectSocket();
	if(r == -1)
	{
		cout<<"connect error"<<endl;
		return -1;
	}

	char buf[1024];
	char szbuf[20] = "data from client ";

	int i = 0;
	int readlen = 0;

	while(true)
	{
		getchar();
		cout<<"next:"<<endl;
		memset(buf,0,1024);
		sprintf(buf,"%s%d",szbuf,i);
		sock->WriteSocket((uint8_t*)buf,strlen(buf));
		cout<<"the "<<i<<" time send:"<<endl;

		memset(buf,0,1024);
		sock->ReadSocket(1024,(uint8_t*)buf,&readlen);
		cout<<"read:"<<buf<<endl;
		i++;
	}

	delete sock;
	return 0;
}