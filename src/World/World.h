#ifndef _World_h_
#define _World_h_

#include <stdio.h>
#include <stdlib.h>

#include "ChunkController.h"

#include "../Camera.h"

class World {
private:

	ChunkController *chunkController;

	Camera *camera;

	bool loaded_area_changed(bool should_fill);
public:
	std::string filename;

	World(ChunkController *chunkController, Camera *camera);
	void update();
	void gen_new_chunks(int x, int y);
};

#endif