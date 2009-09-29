#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////////
// map
//////////////////////////////////////////////////////////////////////////
#define TILE_SZ 32
#define MAP_X 20
#define MAP_Y 10
#define WALKABLE ' '
#define DESTRUCTIBLE '1'

class Map{
public:

	Map();
	~Map();

	bool Load(const char* name);

	void Reset(); // rebuilds walls & etc., to start state

	bool IsWalkable(int x, int y);
	bool IsDestructible(int x, int y);
	void MapSet(int x, int y, char c);

	char Get(int x, int y);
	int* GetPlayerPos();
	int* GetEnemyPos();

private:

	std::string mapname;

	// [0] - cellx, [1] - celly
	int player_pos[2];// stores starting pos - does not change
	int enemy_pos[2];// stores starting pos - does not change

	// map
	char map_orig[MAP_X*MAP_Y]; // does not change
	char map[MAP_X*MAP_Y];
};