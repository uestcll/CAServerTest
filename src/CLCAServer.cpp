#include "CLCAServer.h"
#include "CLCAAddressIPV4.h"
#include "CLLogger.h"
#include "CLCASerializer.h"
#include "CLCADeSerializer.h"
#include "CLSocket.h"
#include "CLDataReceviver.h"
#include "CLProtocolDecapsulator.h"
#include "CLBuffer.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

CLCAServer::CLCAServer(const uint8_t* IP,const uint16_t Port,const int IPType /* = AF_INET */,const int streamType /* = SOCK_STREAM */)
{
	m_sock = new CLSocket(IP,Port,false,IPType,streamType);
	map_ptdpl = new map<int,CLIENT_RECEVIVER_INFO*>;
	map_ptifo = new map<int,CLIENT_POST_INFO*>;
	map_Ser = new map<uint32_t,CLCASerializer*>;
	map_DeSer = new map<uint32_t,CLCADeSerializer*>;
}

CLCAServer::~CLCAServer()
{
	delete m_sock;

	map<int,CLIENT_RECEVIVER_INFO*>::iterator it;
	CLIENT_RECEVIVER_INFO* info = 0;
	CLSocket* sock = 0;
	CLProtocolDecapsulator* ptdpl = 0;
	for(it = map_ptdpl->begin();it != map_ptdpl->end();it++)
	{
		info = it->second;

		if(info != 0)
		{
			sock = info->m_sock;
			ptdpl = info->ptdpl;

			if(sock != 0){
				delete sock;
				sock = 0;
			}

			if(ptdpl != 0)
			{
				delete ptdpl;
				ptdpl = 0;
			}

			free(info);
			info = 0;
		}

		close(it->first); //
	}

	delete map_ptdpl;

	map<uint32_t,CLCASerializer*>::iterator serit;
	CLCASerializer* ser = 0;
	for(serit = map_Ser->begin();serit != map_Ser->end();serit++)
	{
		ser = serit->second;
		if(ser != 0)
			delete ser;
	}

	delete map_Ser;

	map<uint32_t,CLCADeSerializer*>::iterator deserit;
	CLCADeSerializer* deser = 0;
	for(deserit = map_DeSer->begin();deserit != map_DeSer->end();deserit++)
	{
		deser = deserit->second;
		if(deser != 0)
			delete deser;
	}

	delete map_DeSer;
}

int CLCAServer::RegisterSerializer(uint32_t MsgId,CLCASerializer* ser)
{
	if(ser == 0)
		return -1;

	map<uint32_t,CLCASerializer*>::iterator it;
	
	it = map_Ser->find(MsgId);
	if(it != map_Ser->end())
	{
		CLLogger::WriteLogMsg("In CLCAServer::RegisterSerializer(),register serializer error",0);
		return -1;
	}

	map_Ser->insert(pair<uint32_t,CLCASerializer*>(MsgId,ser));

	return 0;
}

int CLCAServer::RegisterDeSerializer(uint32_t MsgId,CLCADeSerializer* deser)
{
	if(deser == 0)
		return -1;

	map<uint32_t,CLCADeSerializer*>::iterator it;

	it = map_DeSer->find(MsgId);
	if(it != map_DeSer->end())
	{
		CLLogger::WriteLogMsg("In CLCAServer::RegisterDeSerializer(),register deserializer error",0);
		return -1;
	}
	map_DeSer->insert(pair<uint32_t,CLCADeSerializer*>(MsgId,deser));

	return 0;
}