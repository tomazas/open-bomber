#include "map.h"
#include "game.h"
#include <cassert>

extern Game* game;

//-----------------------------------------------------------------------------
Map::Map()
{
	memset(map, 0, sizeof map);
	memset(map_orig, 0, sizeof map_orig);
	memset(player_pos, 0, sizeof player_pos);
	memset(enemy_pos, 0, sizeof enemy_pos);
}
//-----------------------------------------------------------------------------
Map::~Map()
{
}
//-----------------------------------------------------------------------------
int* Map::GetPlayerPos()
{
	return enemy_pos;
}
//-----------------------------------------------------------------------------
int* Map::GetEnemyPos()
{
	return player_pos;
}
//-----------------------------------------------------------------------------
void Map::Reset()
{
	memcpy(map, map_orig, sizeof map_orig);
}
//-----------------------------------------------------------------------------
char Map::Get(int x, int y)
{
	assert(x>=0);
	assert(y>=0);
	assert(x < MAP_X);
	assert(y < MAP_Y);
	return map[y*MAP_X + x];
}
//-----------------------------------------------------------------------------
bool Map::Load(const char* name)
{
	memset(map, 0, sizeof map);
	FILE* fp = fopen(name, "rt");
	if(!fp) return false;

	int i = 0;
	while(!feof(fp) && i < MAP_X*MAP_Y)
	{
		char c = fgetc(fp);
		if(!strchr("\n\t\r", c))
		{
			if(c == 'D')// our player pos
			{ 
				c = WALKABLE;
				player_pos[0] = i % MAP_X;
				player_pos[1] = i / MAP_X;
			}
			else if(c == 'E') // enemy pos
			{
				c = WALKABLE;
				enemy_pos[0] = i % MAP_X;
				enemy_pos[1] = i / MAP_X;
			}

			map[i++] = c;
		}
	}

	fclose(fp);

	mapname = name;

	// store
	memcpy(map_orig, map, sizeof(map));

	return true;
}
//-----------------------------------------------------------------------------
bool Map::IsWalkable(int x, int y)
{
	assert(x>=0);
	assert(y>=0);
	assert(x < MAP_X);
	assert(y < MAP_Y);
	return (map[y*MAP_X + x] == WALKABLE && game->IsBombPlanted(x*TILE_SZ, y*TILE_SZ) == false);
}
//-----------------------------------------------------------------------------
bool Map::IsDestructible(int x, int y)
{
	assert(x>=0);
	assert(y>=0);
	assert(x < MAP_X);
	assert(y < MAP_Y);
	return (map[y*MAP_X + x] == DESTRUCTIBLE);
}
//-----------------------------------------------------------------------------
void Map::MapSet(int x, int y, char c)
{
	assert(x>=0);
	assert(y>=0);
	assert(x < MAP_X);
	assert(y < MAP_Y);
	map[y*MAP_X + x] = c;
}