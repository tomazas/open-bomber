#include "Elf.h"
#include "ElfExtra.h"
#include "game.h"

#pragma comment(lib, "Elf2D.lib")
#pragma comment(lib, "winmm.lib")

Game* game = NULL;

void myGameUpdate(const float deltaTime)
{
	game->GameUpdate(deltaTime);
}
// ------------------------------------------------------------------
void myGameRender(const float deltaTime)
{
	game->GameRender();
}
//-----------------------------------------------------------------------------
void myGameShutDown()
{
	game->GameShutdown();
}
// ------------------------------------------------------------------
int main(int argc, char* argv[])
{		
	elfConfig_SetUpdateCallback(myGameUpdate);
	elfConfig_SetRenderCallback(myGameRender);
	elfConfig_SetShutdownCallback(myGameShutDown);
	
	if (elfSystem_Initialise())
	{
		game = new Game();

		elfSystem_Run();

		delete game;
		game = NULL;

		elfSystem_Shutdown();
	}	

	return 0;
}
