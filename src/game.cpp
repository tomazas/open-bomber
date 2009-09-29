#include "Elf.h"
#include "ElfExtra.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <cassert>
#include <string>
#include <stdarg.h>

#include "game.h"
#include "network.h"
#include "map.h"


// textures
int landTiles[LAND_TILES]; // map textures
int bombtile = -1; // texture of bomb
int explotile = -1; // texture of explosion
int dudetile = -1; // dude texture
int enemytile = -1; // blue b-man
int fonttile = -1; // font texture
int maintitle = -1; // main intro
int launchtitle = -1; // server launched - waiting for clients
int jointitle = -1; // join meniu: enter ip

// meniu
int gamestate = MENIU_MAIN;

int main_btn = BTN_LAUNCH_SERVER; // 
std::string ip_string = "";
std::string connect_err = "";


//-----------------------------------------------------------------------------
Game::Game()
{
	playerDead = false;
	enemyDead = false;
	bSendRestart = false;
	deadTime = 0;
	map = NULL;
	net = NULL;

	// bombs
	bombPlantId = 0;
	bombsPlanted = 0;
}
//-----------------------------------------------------------------------------
TBomb Game::GetBomb(int index)
{
	assert(index >= 0 && index < (int)bombs.size());
	return bombs[index];
}
//-----------------------------------------------------------------------------
int Game::GetNumBombs(){
	return bombs.size();
}
//-----------------------------------------------------------------------------
Map* Game::GetMap(){ return map; }
//-----------------------------------------------------------------------------
Network* Game::GetNetwork(){ return net; }
//-----------------------------------------------------------------------------
void Game::FontPrint(int x, int y, const char* str, ...)
{
	char buf[256];

	va_list va;
	va_start(va, str);
	vsnprintf(buf, sizeof buf, str, va);
	va_end(va);

	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);

	const float ts = 1.0f/FONT_SZ;
	int s = 0;

	elfTexture_Bind(fonttile);
	elfQuads_begin();
	for(int i=0; i<(int)strlen(buf); i++)
	{
		char c = buf[i];
		if(c >= 32 && c <= 128)
		{
			int n = c-32;
			int row = n / FONT_SZ;
			int col = n % FONT_SZ;
			elfQuads_setTextureRectangle(col*ts, row*ts+0.1*ts, ts, ts);
			elfQuads_drawRectangle(x+s*FONT_SHIFT, y, FONT_SZ, FONT_SZ);
			s++;
		}
	}
	elfQuads_end();
	elfTexture_Bind(-1);

	elfVideo_EnableBlending(false);
}
//-----------------------------------------------------------------------------
void Game::LoadTextures()
{
	// load font
	elfTexture_Load("font01", "dat/font.tga");
	fonttile = elfTexture_GetId("font01");

	// load main meniu
	elfTexture_Load("meniu1", "dat/title.png");
	maintitle = elfTexture_GetId("meniu1");

	elfTexture_Load("meniu2", "dat/join.png");
	jointitle = elfTexture_GetId("meniu2");

	elfTexture_Load("meniu3", "dat/launch.png");
	launchtitle = elfTexture_GetId("meniu3");


	// load land tiles
	for(int i=0; i<LAND_TILES; i++)
	{
		char buf[64];
		sprintf(buf, "dat/tile%d.png", i);

		if(elfTexture_Load(buf, buf))
		{
			landTiles[i] = elfTexture_GetId(buf);
		}
	}

	// load dude tile
	elfTexture_Load("dude", "dat/dude.tga");
	dudetile = elfTexture_GetId("dude");

	// load enemy tile
	elfTexture_Load("dude2", "dat/dude2.tga");
	enemytile = elfTexture_GetId("dude2");

	// load bomb tile
	elfTexture_Load("bomb", "dat/bomb.tga");
	bombtile = elfTexture_GetId("bomb");

	// load explosion
	elfTexture_Load("xplosion", "dat/xplosion.tga");
	explotile = elfTexture_GetId("xplosion");
}
//-----------------------------------------------------------------------------
void Game::LoadMap(const char* name)
{
	if(map) delete map;
	map = new Map();
	map->Load(name);
}
//-----------------------------------------------------------------------------
void Game::LoadMusic()
{
#ifndef _DEBUG
	elfSoundEffect_Load("bgsnd", "dat/music.wav");
	elfSoundEffect_Fire(elfSoundEffect_GetId("bgsnd"));
#endif
}
//-----------------------------------------------------------------------------
void Game::ResetBombs()
{
	bombPlantId = 0;
	bombsPlanted = 0;
	bombs.clear();
}
//-----------------------------------------------------------------------------
void Game::ResetExplosions()
{
	explo.clear();
}
//-----------------------------------------------------------------------------
void Game::ResetPlayers()
{
	int* p = map->GetPlayerPos();
	player.cellx = p[0];
	player.celly = p[1];
	player.frame = 0;
	player.walk = DUDE_STAND;
	player.x = player.cellx*TILE_SZ;
	player.y = player.celly*TILE_SZ;

	p = map->GetEnemyPos();
	enemy.cellx = p[0];
	enemy.celly = p[1];
	enemy.frame = 0;
	enemy.walk = DUDE_STAND;
	enemy.x = enemy.cellx*TILE_SZ;
	enemy.y = enemy.celly*TILE_SZ;

	playerDead = false;
	enemyDead = false;

	if(net->IsClient())
	{
		// change player position
		p = map->GetEnemyPos();
		player.cellx = p[0];
		player.celly = p[1];
		player.x = player.cellx*TILE_SZ;
		player.y = player.celly*TILE_SZ;
	}
}
//-----------------------------------------------------------------------------
void Game::ResetMeniu()
{
	gamestate = MENIU_MAIN;
	main_btn = BTN_LAUNCH_SERVER;
	ip_string = "";
	connect_err = "";
}
//-----------------------------------------------------------------------------
void Game::ResetGame()
{
	// renew map
	map->Reset();

	ResetPlayers();
	ResetBombs();
	ResetExplosions();
}
//-----------------------------------------------------------------------------
void Game::LoadData()
{
	net = new Network();

	// init
	LoadTextures();
	LoadMap("dat/map.txt");
	LoadMusic();

	ResetMeniu();
	ResetGame();
}
//------------------------------------------------------------------
void Game::sprite(int x, int y, int id, float s, float t, float s2, float t2)
{
	elfTexture_Bind(id);
	elfQuads_begin();
	elfQuads_setTextureRectangle(s,t,s2,t2);
	elfQuads_drawRectangle(x, y, TILE_SZ, TILE_SZ);
	elfQuads_end();
	elfTexture_Bind(-1);
}
//-----------------------------------------------------------------------------
void Game::sprite(int x, int y, int w, int h, int id, float s, float t, float s2, float t2)
{
	elfTexture_Bind(id);
	elfQuads_begin();
	elfQuads_setTextureRectangle(s,t,s2,t2);
	elfQuads_drawRectangle(x, y, w, h);
	elfQuads_end();
	elfTexture_Bind(-1);
}
//-----------------------------------------------------------------------------
bool Game::IsPlayerInside(int cellx, int celly)
{
	// get cell coords
	int x = cellx * TILE_SZ + TILE_SZ/2;
	int y = celly * TILE_SZ + TILE_SZ/2;

	// player center coords
	int player_cx = (player.x + TILE_SZ/2); 
	int player_cy = (player.y + TILE_SZ/2); 

	int dx = x - player_cx;
	if(dx < 0) dx = -dx; // abs
	int dy = y - player_cy;
	if(dy < 0) dy = -dy; // abs

	const int LIMIT = 5; // add some border, so that it wont be so accurate

	if(dx+LIMIT < TILE_SZ && dy+LIMIT < TILE_SZ) return true;

	return false;
}
//-----------------------------------------------------------------------------
void Game::Explode(int cx, int cy)
{
	//assert(numexplo+1 <= MAX_EXPLO); // error
	const float tv = 1.0f/5.0f;

	TExplosion e;
	e.frame = 0;
	e.x = cx*TILE_SZ - 2*TILE_SZ;
	e.y = cy*TILE_SZ - 2*TILE_SZ;
	e.sx = 5 * TILE_SZ; // width
	e.sy = 5 * TILE_SZ; // height
	e.cellx = cx;
	e.celly = cy;
	e.time = 0;
	e.set = true;

	// coords
	e.u = 0;
	e.v = 0;
	e.du = 1.0f;
	e.dv = 1.0f;

	// find all tiles to remove, two for each side
	int startx = cx;// + 2;
	int starty = cy;// + 2;

	// explosion passes to fist tile if: next tile is empty(walkable) or is destructible
	// explosions passes to the second tile if: it is destructible

	// check if the flames reach the player bomberman
	if(IsPlayerInside(startx, starty) && !playerDead)
	{
		playerDead = true;
		deadTime = timeGetTime();
	}

	if((map->IsDestructible(startx+1, starty) || map->IsWalkable(startx+1, starty)) && startx+1 < MAP_X){ // x+1
		map->MapSet(startx+1, starty, WALKABLE);
		//if(IsDestructible(startx+2, starty) && startx+2 < MAP_X) // x+2
		//	MapSet(startx+2, starty, WALKABLE);

		if(IsPlayerInside(startx+1, starty) && !playerDead)
		{
			playerDead = true;
			deadTime = timeGetTime();
		}
	}

	if((map->IsDestructible(startx-1, starty) || map->IsWalkable(startx-1, starty)) && startx-1 >= 0){ // x-1
		map->MapSet(startx-1, starty, WALKABLE);
		//if(IsDestructible(startx-2, starty) && startx-2 >= 0) // x-2
		//	MapSet(startx-2, starty, WALKABLE);

		if(IsPlayerInside(startx-1, starty)&& !playerDead)
		{
			playerDead = true;
			deadTime = timeGetTime();
		}
	}

	if((map->IsDestructible(startx, starty+1) || map->IsWalkable(startx, starty+1)) && starty+1 < MAP_Y){ // y+1
		map->MapSet(startx, starty+1, WALKABLE);
		//if(IsDestructible(startx, starty+2) && starty+2 < MAP_Y) // y+2
		//	MapSet(startx, starty+2, WALKABLE);

		if(IsPlayerInside(startx, starty+1) && !playerDead)
		{
			playerDead = true;
			deadTime = timeGetTime();
		}
	}

	if((map->IsDestructible(startx, starty-1) || map->IsWalkable(startx, starty-1)) && starty-1 >= 0){ // y-1
		map->MapSet(startx, starty-1, WALKABLE);
		//if(IsDestructible(startx, starty-2) && starty-2 >= 0) // y-2
		//	MapSet(startx, starty-2, WALKABLE);

		if(IsPlayerInside(startx, starty-1) && !playerDead)
		{
			playerDead = true;
			deadTime = timeGetTime();
		}
	}

	explo.push_back(e);
}
//-----------------------------------------------------------------------------
void Game::UpdateExplosions(float dt)
{
	for (int i=0; i<explo.size(); i++)
	{
		if(!explo[i].set) break;

		explo[i].time += dt;
		explo[i].frame = (int)(explo[i].time/EXPLO_TICK);

		if(explo[i].frame >= EXPLO_FRAMES) // time to end?
		{
			//explo[i].set = false;

			explo.erase(explo.begin()+i); // remove this 

			i--; // we have shifted, so we check again
		}
	}
}
//-----------------------------------------------------------------------------
void Game::DrawExplosions()
{
	const float t = 1.0f/EXPLO_FRAMES;

	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);

	for (int i=0; i<explo.size(); i++)
	{
		if(!explo[i].set) break;

		sprite(explo[i].x, explo[i].y, 
				explo[i].sx, explo[i].sy, explotile, 
				explo[i].frame*t + explo[i].u, explo[i].v, explo[i].du*t, explo[i].dv);
	}

	elfVideo_EnableBlending(false);
}
//-----------------------------------------------------------------------------
void Game::UpdateBombs(float dt)
{
	// reset bomb explosion markers
	/*std::vector<bool> react;
	react.reserve(bombs.size());
	for(int i=0; i<bombs.size(); i++)
		react[i] = false;*/

	for (int i=0; i<bombs.size(); i++)
	{
		if(!bombs[i].set) break;

		bombs[i].time += dt;

		// pulse the bomb
		if(bombs[i].time - bombs[i].prevtime >= BOMB_TICK)
		{
			bombs[i].frame = (bombs[i].frame + 1) % BOMB_FRAMES;
			bombs[i].prevtime = bombs[i].time;
		}

		// check life left
		if(bombs[i].time >= BOMB_DEAD_TIME)
		{
			// mark the bomb for explosion
			//react[i] = true;

			// check neighbours
			/*for(int j=0; j<bombs.size(); j++)
				if(bombs[j].)*/
			// explode the bomb
			Explode(bombs[i].cellx, bombs[i].celly);

			// the player owns the bomb
			if(bombs[i].owner == BOMB_PLAYER)
				bombsPlanted--;

			// remove this
			bombs.erase(bombs.begin()+i);

			i--; // we have shifted, so check again
		}
	}

	// explode all marked bombs
	//for(int j = bombs.size()-1; j >= 0; j--)
	//{
	//	if(react[j]){
	//		// explode the bomb
	//		Explode(bombs[j].cellx, bombs[j].celly);

	//		// the player owns the bomb
	//		if(bombs[j].owner == BOMB_PLAYER)
	//			bombsPlanted--;

	//		// remove this
	//		bombs.erase(bombs.begin()+j);
	//	}
	//}
}
//-----------------------------------------------------------------------------
bool Game::IsBombPlanted(int x, int y)
{
	for(int i=0; i<bombs.size(); i++)
	{
		if(!bombs[i].set) break; // no active bombs left
		if(bombs[i].x == x && bombs[i].y == y)
			return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
void Game::PlantBomb() // player plants a bomb
{
	if(IsBombPlanted(player.cellx*TILE_SZ, player.celly*TILE_SZ) == false)
	{
		TBomb b;

		b.frame = 0;
		b.set = true;
		b.x = player.cellx*TILE_SZ;
		b.y = player.celly*TILE_SZ;
		b.cellx = player.cellx;
		b.celly = player.celly;
		b.prevtime = 0;
		b.time = 0;
		b.owner = BOMB_PLAYER;
		b.id = bombPlantId++;

		bombs.push_back(b);
		bombsPlanted++;
	}
}
//-----------------------------------------------------------------------------
void Game::AddNewBomb(TBomb b)
{
	bombs.push_back(b);
}
//-----------------------------------------------------------------------------
void Game::DrawBombs()
{
	const float t = 1.0f/(BOMB_FRAMES+1); // extra padding added to the texture

	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);

	for (int i=0; i<bombs.size(); i++)
	{
		if(!bombs[i].set) break;

		sprite(bombs[i].x, bombs[i].y, bombtile, (bombs[i].frame+1)*t, 0, t, 1);
	}

	elfVideo_EnableBlending(false);
}
//-----------------------------------------------------------------------------
bool Game::CanMove(Dude& p, int dx, int dy)
{
	int next_cx = p.cellx+1;
	int next_cy = p.celly+1;
	int prev_cx = p.cellx-1;
	int prev_cy = p.celly-1;

	if(dx > 0 && !map->IsWalkable(next_cx, p.celly)) // moves to the right
	{
		// test x overlap
		int xover = p.x + TILE_SZ;
		int xcell = next_cx*TILE_SZ;
		if(xover > xcell){
			p.x -= xover-xcell;
			return false; // cant move, overlaps
		}
	}
	else if(dx < 0 && prev_cx >=0 && !map->IsWalkable(prev_cx, p.celly))
	{		
		// test x overlap
		int xover = p.x;
		int xcell = prev_cx*TILE_SZ + TILE_SZ;
		if(xover < xcell){
			p.x += xcell-xover;
			return false; // cant move, overlaps
		}
	}
	else if(dy > 0 && !map->IsWalkable(p.cellx, next_cy))
	{
		// test y overlap
		int yover = p.y+TILE_SZ;
		int ycell = next_cy*TILE_SZ;
		if(yover > ycell){
			p.y -= yover-ycell;
			return false; // cant move, overlaps
		}
	}
	else if(dy < 0 && prev_cy >=0 && !map->IsWalkable(p.cellx, prev_cy))
	{
		// test y overlap
		int yover = p.y;
		int ycell = prev_cy*TILE_SZ+TILE_SZ;
		if(yover < ycell){
			p.y += ycell-yover;
			return false; // cant move, overlaps
		}
	}

	return true;
}
//------------------------------------------------------------------
void Game::CheckMoveKeys(Dude& p)
{
	const int H = TILE_SZ/2;
	p.cellx = (p.x+H) / TILE_SZ;
	p.celly = (p.y+H) / TILE_SZ;

	if(elfKeyboard_IsKeyDown(ElfKey::ElfKey_RightArrow) && CanMove(player, 1,0))
	{
		p.x += 1;
		p.walk = DUDE_RIGHT;
	}
	else if(elfKeyboard_IsKeyDown(ElfKey::ElfKey_LeftArrow) && CanMove(player, -1,0))
	{
		p.x -= 1;
		p.walk = DUDE_LEFT;
	}

	if(elfKeyboard_IsKeyDown(ElfKey::ElfKey_UpArrow) && CanMove(player, 0,-1))
	{
		p.y -= 1;
		p.walk = DUDE_UP;
	}
	else if(elfKeyboard_IsKeyDown(ElfKey::ElfKey_DownArrow) && CanMove(player, 0,1))
	{
		p.y += 1;
		p.walk = DUDE_DOWN;
	}

	static bool bSpace = false;
	if(elfKeyboard_IsKeyDown(ElfKey::ElfKey_Space) && bombsPlanted < MAX_BOMBS && !bSpace)
	{
		PlantBomb();
		bSpace = true;
	}
	else if(!elfKeyboard_IsKeyDown(ElfKey::ElfKey_Space))
	{
		bSpace = false;
	}
}
//-----------------------------------------------------------------------------
void Game::UpdateNetwork()
{
	net->ReceivePackets();
	net->CheckPing();
	net->UpdateEnemy();
	net->UpdateNetBombs();
}
//-----------------------------------------------------------------------------
void Game::UpdateDude(float dt)
{
	// update dude animation

	static float tt = 0;
	tt += dt;

	if(tt >= DUDE_FRAME_TICK)
	{
		if(player.walk == DUDE_STAND)
		{		
			player.frame = DUDE_STAND; // same as standing
		}
		else
		{
			// replay dude animation
			player.frame++; // animation

			if(player.frame >= player.walk+3)
				player.frame = player.walk;
		}

		tt = 0;
	}

}
//-----------------------------------------------------------------------------
void Game::DrawMap()
{
	// draw map
	for(int i=0; i<MAP_Y; i++)
	{
		for(int j=0; j<MAP_X; j++)
		{
			switch( map->Get(j,i) )
			{
				case '0': // solid indestructible wall
					sprite(j*TILE_SZ, i*TILE_SZ, landTiles[0], 0, 0, 1, 1);
					break;

				case '1': // destructible bricks
					sprite(j*TILE_SZ, i*TILE_SZ, landTiles[1], 0, 0, 1, 1);
					break;

				default:
					break;
			}	
		}
	}
}
//-----------------------------------------------------------------------------
void Game::DrawPlayers()
{
	static DWORD tick = timeGetTime();
	DWORD tflip = timeGetTime();

	static bool bVisible = true;

	if(tflip - tick >= TICK_INTERVAL){
		bVisible = !bVisible;
		tick = tflip;
	}

	// player
	if(playerDead && !bVisible)
	{
		elfQuads_begin();
		elfQuads_setColour(0x00000000);
		elfQuads_end();
	}

	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);
	float t = 1.0f/NUM_DUDES;
	sprite(player.x, player.y, dudetile, t*player.frame, 0, t, 1);
	elfVideo_EnableBlending(false);

	if(enemyDead && !bVisible)
	{
		elfQuads_begin();
		elfQuads_setColour(0x00000000);
		elfQuads_end();
	}
	else
	{
		elfQuads_begin();
		elfQuads_setColour(0xFFFFFFFF);
		elfQuads_end();
	}

	// enemy
	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);
	sprite(enemy.x, enemy.y, enemytile, t*enemy.frame, 0, t, 1);
	elfVideo_EnableBlending(false);

	elfQuads_begin();
	elfQuads_setColour(0xFFFFFFFF);
	elfQuads_end();
}
//-----------------------------------------------------------------------------
void Game::DrawMainMeniu()
{
	const int startX = SCREENW/2 - TITLEW/2;
	const int startY = SCREENH/2 - TITLEH/2;

	// background
	elfTexture_Bind(maintitle);
	elfQuads_begin();
		elfQuads_setColour(0xFFFFFFFF);
		elfQuads_setTextureRectangle(0,0,1,1);
		elfQuads_drawRectangle(startX, startY, TITLEW, TITLEH);
	elfQuads_end();
	elfTexture_Bind(-1);

#define REL(x, y, w, h) startX+x,startY+y,w,h

	elfVideo_EnableBlending(true);
	elfVideo_SetBlendMode(ElfBM_SrcAlpha, ElfBM_InvSrcAlpha);

	if(main_btn == BTN_LAUNCH_SERVER)
	{
		elfQuads_begin();
			elfQuads_setColour(0x44FF0000);
			elfQuads_setTextureRectangle(0,0,1,1);
			elfQuads_drawRectangle(REL(216, 139, 136, 18));
		elfQuads_end();
	}
	else if(main_btn == BTN_JOIN)
	{
		elfQuads_begin();
			elfQuads_setColour(0x44FF0000);
			elfQuads_setTextureRectangle(0,0,1,1);
			elfQuads_drawRectangle(REL(216, 159, 136, 16));
		elfQuads_end();
	}
	else if(main_btn == BTN_EXIT)
	{
		elfQuads_begin();
			elfQuads_setColour(0x44FF0000);
			elfQuads_setTextureRectangle(0,0,1,1);
			elfQuads_drawRectangle(REL(253, 195, 61, 16));
		elfQuads_end();
	}

	elfVideo_EnableBlending(false);
	
#undef REL

	elfQuads_begin();
	elfQuads_setColour(0xFFFF0000);
	elfQuads_end();

	const int err_x = 192;
	const int err_y = 226;
	FontPrint(startX+err_x, startY+err_y, net->GetStatus().c_str());

	elfQuads_begin();
	elfQuads_setColour(0xFFFFFFFF);
	elfQuads_end();
	
}
//-----------------------------------------------------------------------------
void Game::DrawServerMeniu()
{
	const int startX = SCREENW/2 - TITLEW/2;
	const int startY = SCREENH/2 - TITLEH/2;

	// background
	elfTexture_Bind(launchtitle);
	elfQuads_begin();
		elfQuads_setColour(0xFFFFFFFF);
		elfQuads_setTextureRectangle(0,0,1,1);
		elfQuads_drawRectangle(startX, startY, TITLEW, TITLEH);
	elfQuads_end();
	elfTexture_Bind(-1);
}
//-----------------------------------------------------------------------------
void Game::DrawJoinMeniu()
{
	const int startX = SCREENW/2 - TITLEW/2;
	const int startY = SCREENH/2 - TITLEH/2;

	// background
	elfTexture_Bind(jointitle);
	elfQuads_begin();
		elfQuads_setColour(0xFFFFFFFF);
		elfQuads_setTextureRectangle(0,0,1,1);
		elfQuads_drawRectangle(startX, startY, TITLEW, TITLEH);
	elfQuads_end();
	elfTexture_Bind(-1);

	const int x = 212;
	const int y = 167;

	elfQuads_begin();
	elfQuads_setColour(0xFF5F7DFD);
	elfQuads_end();

	FontPrint(startX+x, startY+y, ip_string.c_str());

	elfQuads_begin();
	elfQuads_setColour(0xFFFF0000);
	elfQuads_end();

	const int err_x = 212;
	const int err_y = 188;
	FontPrint(startX+err_x, startY+err_y, connect_err.c_str());
	
	elfQuads_begin();
	elfQuads_setColour(0xFFFFFFFF);
	elfQuads_end();
}
//-----------------------------------------------------------------------------
void Game::GameUpdate(float deltaTime)
{
	//-----------------------------------------------------------------------------
	// load data
	//-----------------------------------------------------------------------------
	static bool bLoaded = false;
	if(!bLoaded)
	{
		LoadData();
		bLoaded = true;
	}

	if(gamestate == MENIU_PLAYING) // playing
	{
		if(net->IsConnected() == false) // diconnected?
		{
			// bail out
			ResetGame();
			ResetMeniu();
		}
		else
		{
			//-----------------------------------------------------------------------------
			// connected & playing - update
			//-----------------------------------------------------------------------------
			player.walk = DUDE_STAND;

			UpdateNetwork();

			if(playerDead || enemyDead) // one of the players is dead
			{
				if(playerDead && timeGetTime()-deadTime >= DEAD_INTERVAL)
				{
					playerDead = false;
					bSendRestart = true; // send restart to other player
					ResetGame();
				}
			}
			else
			{
				CheckMoveKeys(player);
				UpdateDude(deltaTime);
				UpdateBombs(deltaTime);
				UpdateExplosions(deltaTime);
			}
		}
	}
	else if(gamestate == MENIU_MAIN)
	{
		// move highlight
		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Down))
		{
			main_btn = (main_btn+1)%MAIN_MENIU_NUMBTN;
		}
		else if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Up))
		{
			main_btn--;
			if(main_btn < 0) main_btn = MAIN_MENIU_NUMBTN-1;
		}

		// confirmed option
		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Return))
		{
			switch(main_btn)
			{
				case BTN_JOIN:
					gamestate = MENIU_ENTER_IP;
					break;

				case BTN_LAUNCH_SERVER:
					gamestate = MENIU_SERVER_WAITING;
					net->StartServer();
					break;

				case BTN_EXIT:
					elfSystem_Exit();
					break;

				default: 
					break;
			}
		}
	}
	else if(gamestate == MENIU_SERVER_WAITING)
	{
		net->ReceivePackets();

		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Escape)) // go back
		{
			gamestate = MENIU_MAIN;
			net->StopServer();
		}

		if(net->IsConnected())
		{
			printf("server: connected!");
			gamestate = MENIU_PLAYING; // play game!
		}
	}
	else if(gamestate == MENIU_ENTER_IP)
	{
		net->ReceivePackets();

		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Escape)) // go back
			gamestate = MENIU_MAIN;

		int keys[] = {0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0x34};
		char chrs[] = {'1','2','3','4','5','6','7','8','9','0','.'};
		for(int k = 0; k < 11; k++)
		{
			if(elfKeyboard_IsKeyPressed(keys[k]))
				ip_string += chrs[k];
		}

		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Backspace))
		{
			if(ip_string.length()>0) 
				ip_string.erase(ip_string.length()-1, 1);
		}

		// try to connect
		if(elfKeyboard_IsKeyPressed(ElfKey::ElfKey_Return))
		{
			net->ClientConnect(ip_string.c_str());
		}

		if(net->IsConnected() == false) // failed
		{
			//connect_err = "Failed to connect.";
		}
		else
		{
			// change player position
			int* p = map->GetEnemyPos();
			player.cellx = p[0];
			player.celly = p[1];
			player.x = player.cellx*TILE_SZ;
			player.y = player.celly*TILE_SZ;

			gamestate = MENIU_PLAYING; // play game!

			printf("client: connected!");
		}
	}
}
//-----------------------------------------------------------------------------
void Game::GameRender()
{
	if(gamestate == MENIU_PLAYING)
	{
		elfVideo_Clear(0xFF000000);

		DrawMap();
		DrawBombs();
		DrawExplosions();
		DrawPlayers();

		if(net->IsServerStarted())
			FontPrint(10, 380, "This machine is: SERVER");
		else if(net->IsClient())
			FontPrint(10, 380, "This machine is: CLIENT");
		else
			FontPrint(10, 380, "This machine is: NONE");

		FontPrint(10, 400, "NetStatus: %s", net->GetStatus().c_str());
		FontPrint(10, 420, "ping: %d", net->GetPing());
		FontPrint(10, 440, "bombs: %d", bombs.size());
		FontPrint(10, 460, "explosions: %d", explo.size());
	}
	else if(gamestate == MENIU_MAIN)
	{
		elfVideo_Clear(0xFFFFFFFF);
		DrawMainMeniu();
	}
	else if(gamestate == MENIU_SERVER_WAITING)
	{
		elfVideo_Clear(0xFFFFFFFF);
		DrawServerMeniu();
	}
	else if(gamestate == MENIU_ENTER_IP)
	{
		elfVideo_Clear(0xFFFFFFFF);
		DrawJoinMeniu();
	}
	
}
//-----------------------------------------------------------------------------
void Game::GameShutdown()
{
	if(net->IsServerStarted())
		net->StopServer();
	if(net->IsClient())
		net->ClientDisconnect();
}