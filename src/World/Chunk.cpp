#include "Chunk.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

Chunk::Chunk(int x, int y, int z): x(x), y(y), z(z) {

	std::fill(std::begin(neighbors), std::end(neighbors), nullptr);

	bool has_changed = false;

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
	/*for (int i = 0; i < 6; i++) {
		if (neighbors[i] == nullptr) {
			for (int _x = 0; _x < CHUNK_SIZE; _x++) {
				for (int _y = 0; _y < CHUNK_SIZE; _y++) {
					for (int _z = 0; _z < CHUNK_SIZE; _z++) {
						Block b = { AIR_BLOCK };
						neighbor_blocks[i][_x][_y][_z] = b;
					}
				}
			}
		}
		else {
			for (int _x = 0; _x < CHUNK_SIZE; _x++) {
				for (int _y = 0; _y < CHUNK_SIZE; _y++) {
					for (int _z = 0; _z < CHUNK_SIZE; _z++) {
						
						if (neighbors[i] != nullptr && neighbors[i]->blocks[_x][_y][_z].type != 0 && neighbors[i]->blocks[_x][_y][_z].type != 1) {
							printf("%d\n", neighbors[i]->blocks[_x][_y][_z].type);
						}

						//Block b = { neighbors[i]->blocks[_x][_y][_z].type };

						//neighbor_blocks[i][_x][_y][_z] = b;
					}
				}
			}
		}
	}*/

	renderer.gen_buffer(&blocks, x, y, z, neighbor_blocks);
}

void Chunk::draw(Attrib *attrib) {
	renderer.draw(attrib);
}

bool Chunk::add_neighbor(Chunk *chunk) {

	//neighbor is in the positive direction on the x-axis relative to this chunk
	if (x == chunk->x - CHUNK_SIZE && y == chunk->y && z == chunk->z) {
		neighbors[0] = chunk;
		return true;
	}

	//neighbor is in the negative direction on the x-axis
	else if (x == chunk->x + CHUNK_SIZE && y == chunk->y && z == chunk->z) {
		neighbors[1] = chunk;
		return true;
	}

	//neighbor is in the positive direction on the y-axis
	else if (x == chunk->x && y == chunk->y - CHUNK_SIZE && z == chunk->z) {
		neighbors[2] = chunk;
		return true;
	}

	//neighbor is in the negative direction on the y-axis
	else if (x == chunk->x && y == chunk->y + CHUNK_SIZE && z == chunk->z) {
		neighbors[3] = chunk;
		return true;
	}

	//neighbor is above this chunk
	else if (x == chunk->x && y == chunk->y && z == chunk->z - CHUNK_SIZE) {
		neighbors[4] = chunk;
		return true;
	}
	//neighbor is below this chunk
	else if (x == chunk->x && y == chunk->y && z == chunk->z + CHUNK_SIZE) {
		neighbors[5] = chunk;
		return true;
	}

	return false;
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