#ifndef _GameInstance_h_
#define _GameInstance_h_

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#include "Game.h"

#include "World/ChunkController.h"
#include "PlayerController.h"
#include "World\World.h"

class GameInstance: public Game {
private:
	PlayerController *controller;
	World *world;
	ChunkController *chunkController;
protected:
	virtual void update();
public:
	GameInstance();
};

#endif