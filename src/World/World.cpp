#include "World.h"

#include <algorithm>
#include <iostream>

#include <math.h>

#include "BlockUtil.h"

#include <noise/noise.h>

using namespace noise;
using namespace std;

World::World(ChunkController *chunkController, Camera *camera): chunkController(chunkController), camera(camera) {
	int size = 32;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				//chunkController->add_block(GRASS_BLOCK, i, j, k);
			}
		}
	}

	//chunkController->add_block(GRASS_BLOCK, 0, 0, 0);

	filename = "../../../data/world1.txt";

	chunkController->load_data(filename);
	chunkController->gen_changed_buffers();

	//chunkController->save_data("../../../data/test2.txt");
	//chunkController->save_data(filename);
	//chunkController->read_data("../../../data/test.json");
}

void World::update() {
	if (loaded_area_changed(true)) {
		//chunkController->save_data(filename);
		//chunkController->remove_chunks();
		//chunkController->load_data(filename);
		//if (loaded_area_changed(true)) {
			//chunkController->save_data(filename);
		//}
		chunkController->gen_changed_buffers();
	}
}

void World::gen_new_chunks(int x, int y) {

	module::Perlin myModule;
	//double value = myModule.GetValue(14.50, 20.25, 75.75);

	//int height = 3;

	double changeFactor = .05;
	int scale = 15;
	int dirtCount = 2;
	int stoneCount = 10;
	for (int i = x; i < x + CHUNK_SIZE; i++) {
		for (int j = y; j < y + CHUNK_SIZE; j++) {
			double val = myModule.GetValue(i * changeFactor, j * changeFactor, .50);
			//cout << val << endl;
			int height = (int)(val * scale);
			chunkController->add_generated_block(GRASS_BLOCK, i, j, height);
			for (int k = height - 1; k >= height - dirtCount; k--) {
				chunkController->add_generated_block(DIRT_BLOCK, i, j, k);
			}
			for (int k = height - dirtCount - 1; k >= height - dirtCount - stoneCount; k--) {
				chunkController->add_generated_block(STONE_BLOCK, i, j, k);
			}
		}
	}
}

bool World::loaded_area_changed(bool should_fill) {
	bool has_changed = false;
	int length_side = (std::floor(chunkController->load_distance / CHUNK_SIZE) + 1) * CHUNK_SIZE;
	int nearestChunkX = std::floor(camera->position.x / CHUNK_SIZE) * CHUNK_SIZE;
	int nearestChunkY = std::floor(camera->position.y / CHUNK_SIZE) * CHUNK_SIZE;
	for (int i = nearestChunkX - CHUNK_SIZE - length_side; i <= nearestChunkX + CHUNK_SIZE + length_side; i += CHUNK_SIZE) {
		for (int j = nearestChunkY - CHUNK_SIZE - length_side; j <= nearestChunkY + CHUNK_SIZE + length_side; j += CHUNK_SIZE) {
			if (chunkController->flat_dist_camera(i, j) < chunkController->load_distance && !chunkController->has_chunk(i, j)) {

				if (should_fill) {
					gen_new_chunks(i, j);
					has_changed = true;
				}
				else {
					return true;
				}
			}

		}
	}

	return has_changed;
}