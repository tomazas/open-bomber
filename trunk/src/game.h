#pragma once

#include <map>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////
// textures
//////////////////////////////////////////////////////////////////////////
#define LAND_TILES 2
#define FONT_SZ 16
#define FONT_SHIFT 8

//-----------------------------------------------------------------------------
// meniu
//-----------------------------------------------------------------------------
#define MENIU_MAIN 0
#define MENIU_SERVER_WAITING 1
#define MENIU_ENTER_IP 2
#define MENIU_PLAYING 3

// title
#define SCREENW 640
#define SCREENH 480
#define TITLEW 512
#define TITLEH 256

// title buttons
#define MAIN_MENIU_NUMBTN 3 // numbuttons
#define BTN_LAUNCH_SERVER 0
#define BTN_JOIN 1
#define BTN_EXIT 2

//////////////////////////////////////////////////////////////////////////
// bomberman
//////////////////////////////////////////////////////////////////////////
// walk types
#define DUDE_DOWN 0
#define DUDE_LEFT 3
#define DUDE_RIGHT 6
#define DUDE_UP 9
#define DUDE_STAND 1

#define DUDE_FRAME_TICK 0.2f
#define NUM_DUDES 20 // animation tiles
#define TICK_INTERVAL 200
#define DEAD_INTERVAL 1000

struct Dude{
	int frame; // current animation frame
	int walk;  // walk type
	int x, y;  // current position
	int cellx, celly; // current cell
};



//////////////////////////////////////////////////////////////////////////
// bombs
//////////////////////////////////////////////////////////////////////////
#define MAX_BOMBS 5
#define BOMB_FRAMES 3
#define BOMB_DEAD_TIME 5.0f
#define BOMB_TICK (5.0f/(5*3))
#define BOMB_PLAYER 0
#define BOMB_ENEMY 1

struct TBomb{
	bool set;
	int x, y, cellx, celly;
	float time;
	float prevtime;
	int frame;
	unsigned int id;
	int owner; // 0 - player, 1- enemy
};

//////////////////////////////////////////////////////////////////////////
// explosions
//////////////////////////////////////////////////////////////////////////
#define MAX_EXPLO MAX_BOMBS
#define EXPLO_FRAMES 5
#define EXPLO_TICK 0.1f

struct TExplosion{
	int frame;
	int x, y, cellx, celly;
	int sx, sy;
	float time;
	float u, v, du, dv; // texture coords
	bool set;
};

//-----------------------------------------------------------------------------

class Map;
class Network;

class Game{
public:

	Game();
	~Game(){} // call GameShutdown()

	void GameShutdown();

	// printing text to screen
	void FontPrint(int x, int y, const char* str, ...);

	// loading data
	void LoadData();
	void LoadTextures();
	void LoadMap(const char* name);
	void LoadMusic();
	
	// game reset
	void ResetBombs();
	void ResetExplosions();
	void ResetPlayers();
	void ResetMeniu();
	void ResetGame();
	
	// state checking & map functions
	bool IsBombPlanted(int x, int y);
	bool IsPlayerInside(int cellx, int celly);
	bool IsPlayerDead(){ return playerDead;}
	bool IsEnemyDead(){ return enemyDead; }
	void SetEnemyDead(bool state){ enemyDead = state; }
	bool NeedRestart(){ return bSendRestart; }
	void NeedRestart(bool state){ bSendRestart = state; }
	bool CanMove(Dude& p, int dx, int dy);
	
	Map* GetMap();
	Network* GetNetwork();
	
	// drawing sprites
	void sprite(int x, int y, int id, float s, float t, float s2, float t2);
	void sprite(int x, int y, int w, int h, int id, float s, float t, float s2, float t2);
	
	// game fx
	void Explode(int cx, int cy);
	void PlantBomb();
	void AddNewBomb(TBomb b);
	TBomb GetBomb(int index);
	int GetNumBombs();

	// game update
	void CheckMoveKeys(Dude& p);
	void GameUpdate(float deltaTime);
	void UpdateNetwork();
	void UpdateDude(float dt);
	void UpdateBombs(float dt);
	void UpdateExplosions(float dt);

	// game drawing
	void DrawExplosions();
	void DrawBombs();
	void DrawMap();
	void DrawPlayers();
	void DrawMainMeniu();
	void DrawServerMeniu();
	void DrawJoinMeniu();
	void GameRender();

	// getters/setters
	Dude GetPlayer(){ return player; }
	Dude GetEnemy(){ return enemy; }
	void SetPlayer(Dude& newDude){ player = newDude; }
	void SetEnemy(Dude& newDude){ enemy = newDude; }

private:

	// bomberman
	Dude player;
	Dude enemy;

	bool playerDead;
	bool enemyDead;
	bool bSendRestart;
	unsigned int deadTime;

	Map* map;
	Network* net;

	// bombs
	unsigned int bombPlantId;
	std::vector<TBomb> bombs;
	int bombsPlanted;

	// explosions
	std::vector<TExplosion> explo;

	
};