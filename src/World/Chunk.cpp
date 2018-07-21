#include "Chunk.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

Chunk::Chunk(int x, int y, int z): x(x), y(y), z(z), renderer(false), water_renderer(true) {

	center = glm::ivec3(x + CHUNK_SIZE / 2, y + CHUNK_SIZE / 2, z + CHUNK_SIZE / 2);

	std::fill(std::begin(neighbors), std::end(neighbors), nullptr);

	has_changed = false;

	freshly_generated = false;

	terrain_added = false;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				Block b = { AIR_BLOCK };
				blocks[i][j][k] = b;
			}
		}
	}

}

bool Chunk::has_block(int x, int y, int z) {
	return x >= this->x && x < this->x + CHUNK_SIZE && y >= this->y && y < this->y + CHUNK_SIZE && z >= this->z && z < this->z + CHUNK_SIZE;
}

bool Chunk::add_block(int type, int x, int y, int z) {
	if (has_block(x, y, z)) {

		if (blocks[x - this->x][y - this->y][z - this->z].type != type) {
			set_freshly_generated(false);
			has_changed = true;
			Block b = { type };
			blocks[x - this->x][y - this->y][z - this->z] = b;

			if (x - this->x == CHUNK_SIZE - 1 && neighbors[0] != nullptr) {
				neighbors[0]->has_changed = true;
			}
			else if (x - this->x == 0 && neighbors[1] != nullptr) {
				neighbors[1]->has_changed = true;
			}

			if (y - this->y == CHUNK_SIZE - 1 && neighbors[2] != nullptr) {
				neighbors[2]->has_changed = true;
			}
			else if (y - this->y == 0 && neighbors[3] != nullptr) {
				neighbors[3]->has_changed = true;
			}

			if (z - this->z == CHUNK_SIZE - 1 && neighbors[4] != nullptr) {
				neighbors[4]->has_changed = true;
			}
			else if (z - this->z == 0 && neighbors[5] != nullptr) {
				neighbors[5]->has_changed = true;
			}
		}

		return true;
	}
	else {
		return false;
	}
}

void Chunk::gen_buffer() {

	has_changed = false;

	Block (*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	for (int i = 0; i < NELEMS(neighbors); i++) {
		if (neighbors[i] == nullptr) {
			neighbor_blocks[i] = nullptr;
		}
		else {
			neighbor_blocks[i] = &(neighbors[i]->blocks);
		}
	}

	renderer.gen_buffer(&blocks, x, y, z, neighbor_blocks);
	water_renderer.gen_buffer(&blocks, x, y, z, neighbor_blocks);
}

int Chunk::add_neighbor(Chunk *chunk) {

	//neighbor is in the positive direction on the x-axis relative to this chunk
	if (x == chunk->x - CHUNK_SIZE && y == chunk->y && z == chunk->z) {
		neighbors[0] = chunk;
		return 0;
	}

	//neighbor is in the negative direction on the x-axis
	else if (x == chunk->x + CHUNK_SIZE && y == chunk->y && z == chunk->z) {
		neighbors[1] = chunk;
		return 1;
	}

	//neighbor is in the positive direction on the y-axis
	else if (x == chunk->x && y == chunk->y - CHUNK_SIZE && z == chunk->z) {
		neighbors[2] = chunk;
		return 2;
	}

	//neighbor is in the negative direction on the y-axis
	else if (x == chunk->x && y == chunk->y + CHUNK_SIZE && z == chunk->z) {
		neighbors[3] = chunk;
		return 3;
	}

	//neighbor is above this chunk
	else if (x == chunk->x && y == chunk->y && z == chunk->z - CHUNK_SIZE) {
		neighbors[4] = chunk;
		return 4;
	}
	//neighbor is below this chunk
	else if (x == chunk->x && y == chunk->y && z == chunk->z + CHUNK_SIZE) {
		neighbors[5] = chunk;
		return 5;
	}

	return -1;
}

void Chunk::set_neighbor(Chunk *chunk, int index) {
	neighbors[index] = chunk;
}

bool Chunk::changed() {
	return has_changed;
}

glm::vec3 Chunk::position() {
	return glm::vec3(x, y, z);
}

Block Chunk::get_block(int x, int y, int z) {
	return blocks[reduce_to_fit(x)][reduce_to_fit(y)][reduce_to_fit(z)];
}

int Chunk::reduce_to_fit(int num) {
	while (num < 0) {
		num += CHUNK_SIZE;
	}

	while (num >= CHUNK_SIZE) {
		num -= CHUNK_SIZE;
	}

	return num;
}

void Chunk::set_freshly_generated(bool freshly_generated) {
	this->freshly_generated = freshly_generated;
}

void Chunk::delete_buffer() {
	renderer.delete_buffers();
}