/*
static Camera
static ShaderController
static AssetController

GameInstance: public Game
static ChunkController
world
controller

World: public GameInstance
Controller: public GameInstance

*/

#ifndef _Game_h_
#define _Game_h_

#include <stdio.h>
#include <stdlib.h>

#include "../Camera.h"
#include "ChunkController.h"

class Game {
private:

protected:
	static ChunkController *chunkController;

	static Camera *camera;
public:
	
};

#endif