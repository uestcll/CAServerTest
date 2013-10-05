#include <stdlib.h>
#include <iostream>
#include "CLCAGETPKMessage.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMsgDeSerializer.h"
#include "CLCAServer.h"
#include "CLCAServerManager.h"
#include "CLCAServerByEpoll.h"
#include "CLMessageObserver.h"
#include "CLServerMessageObserver.h"



using namespace std;

int main()
{
	CLCAServerManager* manager = new CLCAServerManager(new CLCAServerByEpoll(0,PORT),new CLServerMessageObserver);
	manager->Initialize();
	manager->RunLoop();
	delete manager;
}