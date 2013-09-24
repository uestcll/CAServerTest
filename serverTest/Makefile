main : ServerMain.o CLCAAddress.o CLCAAddressIPV4.o CLCAClientContext.o CLCAClientPostNetMsgBySocket.o CLCADeSerializer.o CLCAGETPKMessage.o CLCAGETPKMsgDeSerializer.o CLCAGETPKMsgSerializer.o CLCAGETPKMutiMsgDeSerializer.o CLCAGETPKMutiMsgSerializer.o CLCAMessage.o CLCAPostNetMsgTool.o CLCASerializer.o CLCAServer.o CLCAServerByEpoll.o CLCAServerManager.o CLDataReceviver.o CLDataReceviverBySocket.o CLEpoll.o CLMessageObserver.o CLProtocolDecapsulator.o CLServerMessageObserver.o CLSocket.o 
	g++ -I/include -o main ServerMain.o CLCAAddress.o CLCAAddressIPV4.o CLCAClientContext.o CLCAClientPostNetMsgBySocket.o CLCADeSerializer.o CLCAGETPKMessage.o CLCAGETPKMsgDeSerializer.o CLCAGETPKMsgSerializer.o CLCAGETPKMutiMsgDeSerializer.o CLCAGETPKMutiMsgSerializer.o CLCAMessage.o CLCAPostNetMsgTool.o CLCASerializer.o CLCAServer.o CLCAServerByEpoll.o CLCAServerManager.o CLDataReceviver.o CLDataReceviverBySocket.o CLEpoll.o CLMessageObserver.o CLProtocolDecapsulator.o CLServerMessageObserver.o CLSocket.o /libgtest.a -g -lpthread
	rm *.o

ServerMain.o : ServerMain.cpp
	g++ -I/include -o ServerMain.o -c ServerMain.cpp -I../include -g

CLCAAddress.o : ../src/CLCAAddress.cpp
	g++ -o CLCAAddress.o -c ../src/CLCAAddress.cpp -I../include -g

CLCAAddressIPV4.o : ../src/CLCAAddressIPV4.cpp
	g++ -o CLCAAddressIPV4.o -c ../src/CLCAAddressIPV4.cpp -I../include -g

CLCAClientContext.o : ../src/CLCAClientContext.cpp
	g++ -o CLCAClientContext.o -c ../src/CLCAClientContext.cpp -I../include -g

CLCAClientPostNetMsgBySocket.o : ../src/CLCAClientPostNetMsgBySocket.cpp
	g++ -o CLCAClientPostNetMsgBySocket.o -c ../src/CLCAClientPostNetMsgBySocket.cpp -I../include -g

CLCADeSerializer.o : ../src/CLCADeSerializer.cpp
	g++ -o CLCADeSerializer.o -c ../src/CLCADeSerializer.cpp -I../include -g

CLCAGETPKMessage.o : ../src/CLCAGETPKMessage.cpp
	g++ -o CLCAGETPKMessage.o -c ../src/CLCAGETPKMessage.cpp -I../include -g

CLCAGETPKMsgDeSerializer.o : ../src/CLCAGETPKMsgDeSerializer.cpp
	g++ -o CLCAGETPKMsgDeSerializer.o -c ../src/CLCAGETPKMsgDeSerializer.cpp -I../include -g

CLCAGETPKMsgSerializer.o : ../src/CLCAGETPKMsgSerializer.cpp
	g++ -o CLCAGETPKMsgSerializer.o -c ../src/CLCAGETPKMsgSerializer.cpp -I../include -g

CLCAGETPKMutiMsgDeSerializer.o : ../src/CLCAGETPKMutiMsgDeSerializer.cpp
	g++ -o CLCAGETPKMutiMsgDeSerializer.o -c ../src/CLCAGETPKMutiMsgDeSerializer.cpp -I../include -g

CLCAGETPKMutiMsgSerializer.o : ../src/CLCAGETPKMutiMsgSerializer.cpp
	g++ -o CLCAGETPKMutiMsgSerializer.o -c ../src/CLCAGETPKMutiMsgSerializer.cpp -I../include -g

CLCAMessage.o : ../src/CLCAMessage.cpp
	g++ -o CLCAMessage.o -c ../src/CLCAMessage.cpp -I../include -g

CLCAPostNetMsgTool.o : ../src/CLCAPostNetMsgTool.cpp
	g++ -o CLCAPostNetMsgTool.o -c ../src/CLCAPostNetMsgTool.cpp -I../include -g

CLCASerializer.o : ../src/CLCASerializer.cpp
	g++ -o CLCASerializer.o -c ../src/CLCASerializer.cpp -I../include -g

CLCAServer.o : ../src/CLCAServer.cpp
	g++ -o CLCAServer.o -c ../src/CLCAServer.cpp -I../include -g

CLCAServerByEpoll.o : ../src/CLCAServerByEpoll.cpp
	g++ -o CLCAServerByEpoll.o -c ../src/CLCAServerByEpoll.cpp -I../include -g

CLCAServerManager.o : ../src/CLCAServerManager.cpp
	g++ -o CLCAServerManager.o -c ../src/CLCAServerManager.cpp -I../include -g

CLDataReceviver.o : ../src/CLDataReceviver.cpp
	g++ -o CLDataReceviver.o -c ../src/CLDataReceviver.cpp -I../include -g

CLDataReceviverBySocket.o : ../src/CLDataReceviverBySocket.cpp
	g++ -o CLDataReceviverBySocket.o -c ../src/CLDataReceviverBySocket.cpp -I../include -g

CLEpoll.o : ../src/CLEpoll.cpp
	g++ -o CLEpoll.o -c ../src/CLEpoll.cpp -I../include -g

CLMessageObserver.o : ../src/CLMessageObserver.cpp
	g++ -o CLMessageObserver.o -c ../src/CLMessageObserver.cpp -I../include -g

CLProtocolDecapsulator.o : ../src/CLProtocolDecapsulator.cpp
	g++ -o CLProtocolDecapsulator.o -c ../src/CLProtocolDecapsulator.cpp -I../include -g

CLServerMessageObserver.o : ../src/CLServerMessageObserver.cpp
	g++ -o CLServerMessageObserver.o -c ../src/CLServerMessageObserver.cpp -I../include -g

CLSocket.o : ../src/CLSocket.cpp
	g++ -o CLSocket.o -c ../src/CLSocket.cpp -I../include -g

