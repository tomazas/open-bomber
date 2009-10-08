// ------------------------------------------------------------------
// Open-bomber - open-source online bomberman remake
// ------------------------------------------------------------------

#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////////
// map
//////////////////////////////////////////////////////////////////////////
#define TILE_SZ 32          // tile size in pixels
#define MAP_X 20            // number of columns in the map
#define MAP_Y 10            // number of rows in the map
#define WALL '0'            // denotes a inpassable wall
#define WALKABLE ' '        // denotes a walkable tile in map
#define DESTRUCTIBLE '1'    // denotes a destructible tile in map

class Map{
public:

	Map();
	~Map();

	// loads a map from file
	bool Load(const char* name);

	// rebuilds walls & etc., to start state
	void Reset(); 

	// returns true if the cell is walkable
	bool IsWalkable(int cellx, int celly); 
	// returns true if the cell is destructible
	bool IsDestructible(int cellx, int celly); 

	// sets cell type
	void MapSet(int cellx, int celly, char c);
	 // returns cell type
	char Get(int cellx, int celly);

	// returns starting position of player in map
	int* GetPlayerPos();
	// returns starting position of enemy in map
	int* GetEnemyPos();

	// returns the loaded map file name 
	std::string GetFileName(){ return mapname; }

private:

	std::string mapname;

	// [0] - cellx, [1] - celly
	int player_pos[2];// stores starting pos - does not change
	int enemy_pos[2];// stores starting pos - does not change

	// map
	char map_orig[MAP_X*MAP_Y]; // does not change
	char map[MAP_X*MAP_Y];
};