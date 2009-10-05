// ------------------------------------------------------------------
// Open-bomber - open-source online bomberman remake
// ------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////
// textures
//////////////////////////////////////////////////////////////////////////
#define LAND_TILES 2  // number of possible land tiles
#define FONT_SZ 16    // default font size
#define FONT_SHIFT 8  // default gap between font chars

//-----------------------------------------------------------------------------
// meniu
//-----------------------------------------------------------------------------
// meniu states
#define MENIU_MAIN 0        
#define MENIU_SERVER_WAITING 1
#define MENIU_ENTER_IP 2
#define MENIU_PLAYING 3

// title - screen parameters
#define SCREENW 640
#define SCREENH 480
#define TITLEW 512
#define TITLEH 256

// title buttons
#define MAIN_MENIU_NUMBTN 3 // number of buttons in main meniu
#define BTN_LAUNCH_SERVER 0 
#define BTN_JOIN 1
#define BTN_EXIT 2

//////////////////////////////////////////////////////////////////////////
// bomberman
//////////////////////////////////////////////////////////////////////////
// walk types & their respective frames in texture file
#define DUDE_DOWN 0  
#define DUDE_LEFT 3
#define DUDE_RIGHT 6
#define DUDE_UP 9
#define DUDE_STAND 1

#define DUDE_FRAME_TICK 0.2f // walk frame change time
#define NUM_DUDES 20         // number of walk animation tiles
#define TICK_INTERVAL 200    // b-man death animation speed flips visiblity every 0.2s
#define DEAD_INTERVAL 1000   // b-man death animation blink time 1.0s 

struct Dude{
	int frame; // current animation frame
	int walk;  // walk type
	int x, y;  // current position
	int cellx, // current cell column
		celly; // current cell row
};



//////////////////////////////////////////////////////////////////////////
// bombs
//////////////////////////////////////////////////////////////////////////
#define MAX_BOMBS 5            // maximum number of bombs a b-man can place
#define BOMB_FRAMES 3          // number of frames in a bomb texture
#define BOMB_DEAD_TIME 5.0f    // time until bomb explodes
#define BOMB_TICK (BOMB_DEAD_TIME/(5*3)) //bomb pulse speed in seconds
#define BOMB_PLAYER 0          // bomb flag which shows that a bomb belongs to this player
#define BOMB_ENEMY 1           // bomb flag which shows that a bomb belongs to enemy player

struct TBomb{
	bool set;         // true if the bomb is active
	int cellx,        // current cell column
		celly;        // current cell row
	float time;       // bomb timer
	float prevtime;   // previous frame time (needed for elapsed time calc)
	int frame;        // current frame in texture
	unsigned int id;  // unique bomb identifier
	int owner;        // flag which denotes owner of the bomb
};

//////////////////////////////////////////////////////////////////////////
// explosions
//////////////////////////////////////////////////////////////////////////
#define MAX_EXPLO MAX_BOMBS  // maximum number of explosions visible
#define EXPLO_FRAMES 5       // number of explosion frames in the texture
#define EXPLO_TICK 0.1f      // explosion frame change time

struct TExplosion{
	int frame;               // current explosion frame
	int x,                   // top left corner of the quad 
		y,                   // top left corner of the quad
		cellx,               // current center cell column
		celly;               // current center cell row
	int sx,                  // width in pixels 
		sy;                  // height in pixels
	float time;              // explosion life (time passed)
	float u,                 // x texture coord of top left corner
		  v,                 // y texture coord of top left corner
		  du,                // one frame texture delta in x/u direction
		  dv;                // one frame texture delta in y/v direction
	bool set;                // set to true if the explosion is active
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
	bool IsBombPlanted(int cellx, int celly);
	bool IsPlayerInside(int cellx, int celly);
	bool IsPlayerDead();
	bool IsEnemyDead();
	void SetEnemyDead(bool state);
	bool NeedRestart();
	void NeedRestart(bool state);
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
	Dude GetPlayer();
	Dude GetEnemy();
	void SetPlayer(Dude& newDude);
	void SetEnemy(Dude& newDude);

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

	// bombs array
	unsigned int bombPlantId;
	std::vector<TBomb> bombs;
	int bombsPlanted;

	// explosions array
	std::vector<TExplosion> explo;

	
};