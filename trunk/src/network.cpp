// ------------------------------------------------------------------
// Open-bomber - open-source online bomberman remake
// ------------------------------------------------------------------

#include "network.h"
#include <string>
#include <vector>



//-----------------------------------------------------------------------------
DWORD Network::ping = 0;
DWORD Network::ping_start = 0;
bool Network::bPingCheck = false;

extern Game* game;


//-----------------------------------------------------------------------------
Network::Network()
{
	serverStarted = false;
	isClient = false;
	isConnected = false;
	//clientID = -1;
	net_status = "";//"Press S for server, C - for client";

	//ping = 0;
	//ping_start = 0;
	//bPingCheck = false;

	servSock = 0;
	clientSock = 0;
}
//-----------------------------------------------------------------------------
Network::~Network()
{

}
//-----------------------------------------------------------------------------
bool Network::IsServerStarted(){ return serverStarted; }
//-----------------------------------------------------------------------------
bool Network::IsClient(){ return isClient; }
//-----------------------------------------------------------------------------
bool Network::IsConnected(){ return isConnected; }
//-----------------------------------------------------------------------------
std::string Network::GetStatus(){ return net_status; }
//-----------------------------------------------------------------------------
DWORD Network::GetPing(){ return ping; }
//-----------------------------------------------------------------------------
void Network::SendData(const char* func, char* data, int size, SystemAddress addr)
{
	if(isClient)
		clientSock->RPC(func, data, size*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, true, 0, UNASSIGNED_NETWORK_ID,0);
	else
		servSock->RPC(func, data, size*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addr, true, 0, UNASSIGNED_NETWORK_ID,0);
}
//-----------------------------------------------------------------------------
void Network::ClientDisconnect()
{
	if(clientSock)
	{
		RakNetworkFactory::DestroyRakPeerInterface(clientSock);
		clientSock = 0;
	}

	isClient = false;
	isConnected = false;
	net_status = "Game status: Server timeout.";
}
//-----------------------------------------------------------------------------
void Network::ClientConnect(const char* ip)
{
	if(clientSock == 0)
		clientSock = RakNetworkFactory::GetRakPeerInterface();

	if(clientSock)
	{
		clientSock->Startup(1,30,&SocketDescriptor(), 1);
		clientSock->Connect(ip, SERVER_PORT, 0,0);

		isClient = true;
		isConnected = false;

		REGISTER_STATIC_RPC(clientSock, NetUpdatePlayer);
		REGISTER_STATIC_RPC(clientSock, NetUpdateDead);
		REGISTER_STATIC_RPC(clientSock, NetUpdateBombs);
		//REGISTER_STATIC_RPC(clientSock, PingMe);
		REGISTER_STATIC_RPC(clientSock, PongMe);
		REGISTER_STATIC_RPC(clientSock, NetResetGame);
	}
}
//-----------------------------------------------------------------------------
void Network::StartServer()
{
	if(servSock != 0) return; // already created

	servSock = RakNetworkFactory::GetRakPeerInterface();

	if(servSock)
	{
		servSock->Startup(1, 30, &SocketDescriptor(SERVER_PORT, 0), 1);
		servSock->SetMaximumIncomingConnections(1);

		serverStarted = true;
		isClient = false;
		isConnected = false;

		REGISTER_STATIC_RPC(servSock, NetUpdatePlayer);
		REGISTER_STATIC_RPC(servSock, NetUpdateDead);
		REGISTER_STATIC_RPC(servSock, NetUpdateBombs);
		//REGISTER_STATIC_RPC(servSock, PingMe);
		REGISTER_STATIC_RPC(servSock, PongMe);
		REGISTER_STATIC_RPC(servSock, NetResetGame);
	}
}
//-----------------------------------------------------------------------------
void Network::StopServer()
{
	if(servSock)
	{
		RakNetworkFactory::DestroyRakPeerInterface(servSock);
		servSock = 0;
	}

	serverStarted = false;
	isConnected = false;
}
//-----------------------------------------------------------------------------
void Network::ReceivePackets()
{
	if(clientSock == 0 && servSock == 0) return;

	RakPeerInterface* peer = 0;

	if(isClient)
		peer = clientSock;
	else
		peer = servSock;

	// receive data
	Packet* packet = peer->Receive();

	while(packet)
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			//printf("Another client has disconnected.\n");
			isConnected = false;
			break;
		case ID_REMOTE_CONNECTION_LOST:
			//printf("Another client has lost the connection.\n");
			isConnected = false;
			break;

		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			//printf("Another client has connected.\n");
			isConnected = true;
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			//printf("Our connection request has been accepted.\n");
			isConnected = true;
			break;
		case ID_NEW_INCOMING_CONNECTION:
			//printf("A connection is incoming.\n");
			isConnected = true;
			break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			//printf("The server is full.\n");
			net_status = "The server is full.";
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			if (isClient == false){
				//printf("A client has disconnected.\n");
			} else {
				//printf("We have been disconnected.\n");
			}
			net_status = "Have been disconnected.";
			isConnected = false;
			break;
		case ID_CONNECTION_LOST:
			if (isClient == false){
				//printf("A client lost the connection.\n");
			} else {
				//printf("Connection lost.\n");
			}
			net_status = "Connection lost.";
			isConnected = false;
			break;

		default:
			//printf("Message with identifier %i has arrived.\n", packet->data[0]);
			if(packet->data[0] == 15){ 
				net_status = "Failed to connect.";
				isConnected = false;
			}
			break;
		}

		peer->DeallocatePacket(packet);
		packet = peer->Receive(); // get new packet
	}
}
//-----------------------------------------------------------------------------
void Network::CheckPing()
{
	if(!isConnected) return;

	if(bPingCheck)
	{
		DWORD elapsed = timeGetTime() - ping_start;
		if(elapsed > MAX_PING_CHECK)
		{
			ping = PING_NO_RESPONSE; // high ping 
			bPingCheck = false;
		}

		return;
	}

	// start ping check
	//SendData(CLASS_MEMBER_ID(Network, PingMe), 0, 0);
	PingMe();
	ping_start = timeGetTime();
	bPingCheck = true;
}
//-----------------------------------------------------------------------------
void Network::PingMe()
{
	// send result back
	SendData("PongMe", 0, 0);
}
//-----------------------------------------------------------------------------
void Network::PongMe(RPCParameters* params)
{
	// received pong
	if(bPingCheck)
	{
		ping = timeGetTime() - ping_start;
		bPingCheck = false; // end of ping check
	}
}
//-----------------------------------------------------------------------------
void Network::UpdateEnemy()
{
	if(!isConnected) return;

	Dude player = game->GetPlayer();
	bool playerDead = game->IsPlayerDead();
	SendData("NetUpdatePlayer", (char*)&player, sizeof player);
	SendData("NetUpdateDead", (char*)&playerDead, sizeof playerDead);
	
	if(game->NeedRestart()){
		SendData("NetResetGame", 0, 0);
		game->NeedRestart( false );
	}
}
//-----------------------------------------------------------------------------
void Network::UpdateNetBombs()
{
	if(!isConnected) return;

	// count the bombs
	int temp = 0;
	for(int i=0; i<game->GetNumBombs(); i++) 
		if(game->GetBomb(i).owner == BOMB_PLAYER) temp++;

	// create buffer
	int sz = sizeof(temp) + sizeof(TBomb) * temp; 
	char* buf = new char[sz];

	if(buf)
	{
		// add count
		char* ptr = buf;
		memcpy(ptr, &temp, sizeof(temp));
		ptr += sizeof(temp);

		// format the packet buffer
		for(int i=0; i<game->GetNumBombs(); i++) 
		{
			if(game->GetBomb(i).owner == BOMB_PLAYER)
			{
				memcpy(ptr, &game->GetBomb(i), sizeof(TBomb));
				ptr += sizeof(TBomb);
			}
		}

		// send bombs
		SendData("NetUpdateBombs", buf, sz);

		delete [] buf;
	}
}

//-----------------------------------------------------------------------------
void UpdateWithNewBomb(TBomb b)
{
	if(game->IsBombPlanted(b.cellx, b.celly) == false)
	{
		b.owner = BOMB_ENEMY;
		game->AddNewBomb(b);
	}
}
//-----------------------------------------------------------------------------
void NetResetGame(RPCParameters* params)
{
	game->ResetGame();
}
//-----------------------------------------------------------------------------
void NetUpdatePlayer(RPCParameters* params)
{
	if(params->numberOfBitsOfData > 0){
		Dude enemy;
		memcpy(&enemy, params->input, sizeof enemy);
		game->SetEnemy(enemy);
	}
}
//-----------------------------------------------------------------------------
void NetUpdateDead(RPCParameters* params)
{
	if(params->numberOfBitsOfData > 0){
		bool enemyDead;
		memcpy(&enemyDead, params->input, sizeof enemyDead);
		game->SetEnemyDead(enemyDead);
	}
}
//-----------------------------------------------------------------------------
void NetUpdateBombs(RPCParameters* params)
{
	if(params->numberOfBitsOfData > 0)
	{
		// get count
		int num = 0;
		memcpy(&num, params->input, sizeof(int));

		unsigned char* p = params->input;
		p += sizeof(int); // skip size

		for(int i=0; i<num; i++)
		{
			TBomb bTemp;
			memcpy(&bTemp, p, sizeof(TBomb));
			UpdateWithNewBomb(bTemp);

			p += sizeof(TBomb);
		}
	}
}