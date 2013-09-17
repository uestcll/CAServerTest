#include <stdlib.h>
#include <iostream>
#include "CLCAGETPKMessage.h"
#include "CLCAGETPKMsgSerializer.h"
#include "CLCAGETPKMsgDeSerializer.h"
#include "CLCAServer.h"
#include "CLCAServerManager.h"
using namespace std;

int main()
{
	CLCAServerManager* manager = new CLCAServerManager(new CLCAServerByEpoll(0,PORT));
	manager->Initialize();
	manager->RunLoop();
	delete manager;
}