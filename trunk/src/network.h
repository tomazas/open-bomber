#pragma once


//-----------------------------------------------------------------------------
// server & client
//-----------------------------------------------------------------------------

#define SERVER_PORT 7411
//#define CLIENT_PORT 5488

#define MAX_PING_CHECK 1000 // time to wait for ping-pong in ms
#define PING_NO_RESPONSE 9999

#define MSG_PING 101
#define MSG_PONG 102
#define MSG_SEND_PLAYER 103
#define MSG_UPDATE_PLAYER 104
#define MSG_SEND_MAP 105
#define MSG_UPDATE_MAP 106
#define MSG_SEND_BOMBS 107
#define MSG_UPDATE_BOMBS 108

//-----------------------------------------------------------------------------
// server & client
//-----------------------------------------------------------------------------

//#include "Elf.h"

#include <string>
#include <Windows.h>

#include "game.h"
#include "raknet.h"


void UpdateWithNewBomb(TBomb b);

void NetResetGame(RPCParameters* params);
void NetUpdatePlayer(RPCParameters* params);
void NetUpdateDead(RPCParameters* params);
void NetUpdateBombs(RPCParameters* params);

void PongMe(RPCParameters* params);

class Network{
public:

	Network();
	~Network();

	void ClientDisconnect();
	void ClientConnect(const char* ip);

	void StartServer();
	void StopServer();

	void ReceivePackets();
	void CheckPing();
	void UpdateEnemy();
	void UpdateNetBombs();

	bool IsServerStarted(){ return serverStarted; }
	bool IsClient(){ return isClient; }
	bool IsConnected(){ return isConnected; }
	//ElfClientId GetClientId(){ return clientID; }
	std::string GetStatus(){ return net_status; }
	DWORD GetPing(){ return ping; }

private:

	void SendData(const char* func, char* data, int size, SystemAddress addr = UNASSIGNED_SYSTEM_ADDRESS);
	void PingMe();
	static void PongMe(RPCParameters* params);

	bool serverStarted;
	bool isClient;
	bool isConnected;
	//ElfClientId clientID;
	std::string net_status;

	static DWORD ping;
	static DWORD ping_start;
	static bool bPingCheck;

	RakPeerInterface *servSock;
	RakPeerInterface *clientSock;;
};