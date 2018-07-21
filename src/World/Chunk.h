#ifndef _Chunk_h_
#define _Chunk_h_

#include <vector>
#include <array>

#include <GL/glew.h>

#include "../util.h"

#include "../Render/BlockRenderer.h"

#include "BlockUtil.h"

#include <glm/glm.hpp>

class Chunk {
private:
	int x;
	int y;
	int z;

	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	const int size = CHUNK_SIZE;

	Chunk* neighbors[6];

	//this variable tells if the chunk has changed since its buffer was last updated
	bool has_changed;

	int reduce_to_fit(int num);

	//this variable tells if this chunk or chunks above/below it have ever been edited
	bool freshly_generated;

public:
	glm::ivec3 center;

	BlockRenderer renderer;
	BlockRenderer water_renderer;
	
	void set_freshly_generated(bool freshly_generated);
	bool get_freshly_generated() { return freshly_generated; };

	Chunk(int x, int y, int z);
	void gen_buffer();

	void delete_buffer();

	bool has_block(int x, int y, int z);
	bool add_block(int type, int x, int y, int z);

	Block get_block(int x, int y, int z);

	int add_neighbor(Chunk *chunk);

	void set_neighbor(Chunk *chunk, int index);

	bool changed();

	glm::vec3 position();

	bool terrain_added;
};

#endif