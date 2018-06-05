#ifndef _BlockRenderer_h_
#define _BlockRenderer_h_

#include <vector>

#include <GL/glew.h>

#include "IRenderer.h"

#include "../util.h"

#include "../World/BlockUtil.h"

class BlockRenderer {
private:
	GLuint vbo;
	GLuint ebo;
	int elements_size;
	int vertices_size;

	const int vertex_size = 5;

	void gen_vertices_buffer(std::vector<GLfloat> *vertices);
	void gen_elements_buffer(std::vector<GLuint> *elements);

public:
	BlockRenderer();
	void gen_buffer(Block (*blocks)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], int offsetX, int offsetY, int offsetZ, Block(*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
	void draw(Attrib *attrib);
};

#endif