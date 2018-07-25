#ifndef _BlockRenderer_h_
#define _BlockRenderer_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "IRenderer.h"

#include "../util.h"

#include "../World/BlockUtil.h"

class BlockRenderer : public IRenderer {
private:
	bool is_water;

public:
	BlockRenderer(bool is_water);
	void gen_buffer(Block (*blocks)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], int offsetX, int offsetY, int offsetZ, Block(*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);

	void draw_transparent(std::string programKey, std::string textureKey, glm::vec3 player_pos);
};

#endif