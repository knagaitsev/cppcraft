#ifndef _World_h_
#define _World_h_

#include <stdio.h>
#include <stdlib.h>

#include <noise/noise.h>

#include <glm/glm.hpp>

#include "ChunkController.h"

#include "../Camera.h"

#include "../Controller.h"

class World {
private:

	ChunkController *chunkController;

	Camera *camera;

	Controller *controller;

	bool loaded_area_changed(bool should_fill);

	noise::module::Perlin perlinModule;

	int get_height(int x, int y);
	void gen_default_tree(int x, int y);
	void gen_pine_tree(int x, int y);
	void gen_oak_tree(int x, int y);

	void add_on_line(int type, glm::vec3 p1, glm::vec3 p2);

	int chunks_genned;

	int buffer_gen_count;

	bool awaiting_buffer_gen;

	bool initial_load;

public:
	std::string filename;

	World(ChunkController *chunkController, Camera *camera, Controller *controller);
	void update();
	void gen_new_chunks(int x, int y);
};

#endif