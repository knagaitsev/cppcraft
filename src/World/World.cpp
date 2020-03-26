#include "World.h"

#include <algorithm>
#include <iostream>

#include <math.h>

#include "BlockUtil.h"

#include "proctree.h"

using glm::vec3;

using namespace glm;

using namespace noise;
using namespace std;

World::World(ChunkController *chunkController, PlayerController *controller): chunkController(chunkController), controller(controller) {

	camera = Game::camera;

	buffer_gen_count = 0;
	awaiting_buffer_gen = false;

	initial_load = true;

	int size = 32;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				//chunkController->add_block(GRASS_BLOCK, i, j, k);
			}
		}
	}

	//chunkController->add_block(GRASS_BLOCK, 0, 0, 0);

	filename = "../worlds/world1.txt";

	chunkController->load_data(filename);
	chunkController->gen_changed_buffers();

	//chunkController->save_data("../../../data/test2.txt");
	//chunkController->save_data(filename);
	//chunkController->read_data("../../../data/test.json");

	

}

void World::update() {
	if (loaded_area_changed(true)) {
		//chunkController->save_data(filename);
		chunkController->remove_chunks();
		//chunkController->load_data(filename);
		//if (loaded_area_changed(true)) {
			//chunkController->save_data(filename);
		//}
		awaiting_buffer_gen = true;
		initial_load = false;
		return;
	}

	if (awaiting_buffer_gen) {
		chunkController->gen_changed_buffers();
		awaiting_buffer_gen = false;
	}
}

void World::gen_new_chunks(int x, int y) {

	module::Perlin myModule;
	//double value = myModule.GetValue(14.50, 20.25, 75.75);

	//int height = 3;

	int dirtCount = 2;
	int stoneCount = 10;
	for (int i = x; i < x + CHUNK_SIZE; i++) {
		for (int j = y; j < y + CHUNK_SIZE; j++) {
			int height = get_height(i, j);
			if (height < 0) {
				chunkController->add_generated_block(SAND_BLOCK, i, j, height);
			}
			else {
				chunkController->add_generated_block(GRASS_BLOCK, i, j, height);
			}
			for (int k = height - 1; k >= height - dirtCount; k--) {
				chunkController->add_generated_block(DIRT_BLOCK, i, j, k);
			}
			for (int k = height - dirtCount - 1; k >= height - dirtCount - stoneCount; k--) {
				chunkController->add_generated_block(STONE_BLOCK, i, j, k);
			}

			int below = height;
			while (below < 0) {
				below++;
				chunkController->add_generated_block(WATER_BLOCK, i, j, below);
			}
			gen_pine_tree(i, j);
		}
	}
}

void World::gen_default_tree(int x, int y) {
	int square_range = 2;
	for (int rep = 0; rep < 2; rep++) {
		for (int i = x - square_range; i <= x + square_range; i++) {
			for (int j = y - square_range; j <= y + square_range; j++) {
				double val = perlinModule.GetValue(i, j, 10.30);
				bool tree = val > 0.8 && get_height(i, j) > 0;
				if (tree) {
					int height = get_height(i, j) + 1;
					int distX = ABS(x - i);
					int distY = ABS(y - j);
					if (distX == 0 && distY == 0) {
						if (rep == 0) {
							chunkController->add_generated_block(LOG_BLOCK, x, y, height);
							chunkController->add_generated_block(LOG_BLOCK, x, y, height + 1);
							chunkController->add_generated_block(LOG_BLOCK, x, y, height + 2);
							chunkController->add_generated_block(LOG_BLOCK, x, y, height + 3);
							chunkController->add_generated_block(LOG_BLOCK, x, y, height + 4);
						}
						else {
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 5);
						}
					}
					else if (distX <= 1 && distY <= 1) {
						if (rep == 1) {
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 3);
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 4);
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 5);
						}
					}
					else if (distX == 2 && distY == 2) {
						if (rep == 1) {
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 3);
						}
					}
					else if (distX <= 2 && distY <= 2) {
						if (rep == 1) {
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 3);
							chunkController->add_generated_block(LEAF_BLOCK, x, y, height + 4);
						}
					}
				}
			}
		}
	}
}

void World::gen_pine_tree(int x, int y) {
	double val = perlinModule.GetValue(x, y, 10.30);
	double val2 = perlinModule.GetValue(x, y, 20.30);
	double val3 = perlinModule.GetValue(x, y, 30.30);

	int groundHeight = get_height(x, y);
	bool tree = val > 0.88 && get_height(x, y) > 0;
	if (tree) {

		int treeHeight = floor((val2 + 4) * 4);
		for (int d = 1; d <= treeHeight + 3; d++) {
			double radius = 4.0 * (1 - ((double)d) / (treeHeight + 3)) + .7;

			if (d <= treeHeight) {
				chunkController->add_generated_block(LOG_BLOCK, x, y, groundHeight + d);
			}
			else {
				chunkController->add_generated_block(LEAF_BLOCK, x, y, groundHeight + d);
			}

			if (d > 3) {
				for (int i = -ceil(radius); i <= ceil(radius); i++) {
					for (int j = -ceil(radius); j <= ceil(radius); j++) {
						double leaf_val = perlinModule.GetValue(x + i, y + j, 40.30 + d * 3);
						if (i * i + j * j <= radius * radius && leaf_val < .3 * (((double)d) / treeHeight)) {
							chunkController->add_generated_block(LEAF_BLOCK, x + i, y + j, groundHeight + d);
						}
					}
				}
			}
		}
	}
}

void World::gen_oak_tree(int x, int y) {
	double val = perlinModule.GetValue(x, y, 10.30);
	double val2 = perlinModule.GetValue(x, y, 20.30);
	double val3 = perlinModule.GetValue(x, y, 30.30);

	int groundHeight = get_height(x, y);
	bool tree = val > 0.9999 && get_height(x, y) > 0;
	if (tree) {
		Proctree::Tree tree;

		tree.mProperties.mSeed = x * y;
		//tree.mProperties.mTreeSteps = 7;
		tree.mProperties.mLevels = 3;
		tree.mProperties.mInitialBranchLength = 1.89f;
		tree.mProperties.mRadiusFalloffRate = .3f;
		tree.mProperties.mTrunkLength = 3.5f;

		tree.mProperties.mTwigScale = .4f;

		tree.generate();

		int scale = 4;

		int depth_in_ground = 5;

		for (int i = 0; i < tree.mFaceCount; i ++) {
			Proctree::fvec3 proc_p1 = tree.mVert[tree.mFace[i].x];
			Proctree::fvec3 proc_p2 = tree.mVert[tree.mFace[i].y];
			Proctree::fvec3 proc_p3 = tree.mVert[tree.mFace[i].z];

			vec3 p1 = vec3(x + proc_p1.x * scale, y + proc_p1.z * scale, groundHeight - depth_in_ground + proc_p1.y * scale);
			vec3 p2 = vec3(x + proc_p2.x * scale, y + proc_p2.z * scale, groundHeight - depth_in_ground + proc_p2.y * scale);
			vec3 p3 = vec3(x + proc_p3.x * scale, y + proc_p3.z * scale, groundHeight - depth_in_ground + proc_p3.y * scale);

			add_on_line(LOG_BLOCK, p1, p2);
			add_on_line(LOG_BLOCK, p2, p3);

			//vec3 pt = vec3(round(tree.mVert[i].x * 10), round(tree.mVert[i].y * 10), round(tree.mVert[i].z * 10));
			/*bool exists = false;
			for (vec3 pt2 : points) {
				if (pt.x == pt2.x && pt.y == pt2.y && pt.z == pt2.z) {
					exists = true;
				}
			}
			if (!exists) {
				points.push_back(pt);
			}*/
		}

		std::vector<ivec3> points;

		for (int i = 0; i < tree.mTwigVertCount; i++) {
			
			ivec3 pt = ivec3(x + round(tree.mTwigVert[i].x * scale), y + round(tree.mTwigVert[i].z * scale), groundHeight - depth_in_ground + round(tree.mTwigVert[i].y * scale));
			bool exists = false;
			for (ivec3 pt2 : points) {
				if (pt.x == pt2.x && pt.y == pt2.y && pt.z == pt2.z) {
					exists = true;
				}
			}
			if (!exists) {
				points.push_back(pt);
			}
		}

		for (int i = 0; i < points.size(); i++) {
			int range = 3;
			float radius = 3.0f;
			for (int xl = points[i].x - range; xl <= points[i].x + range; xl++) {
				for (int yl = points[i].y - range; yl <= points[i].y + range; yl++) {
					for (int zl = points[i].z - range; zl <= points[i].z + range; zl++) {
						double chance_val = perlinModule.GetValue(xl, yl, 30.30 + zl);
						if (glm::distance(vec3(xl, yl, zl), vec3(points[i])) <= radius && chance_val > 0.38) {
							chunkController->add_generated_block(LEAF_BLOCK, vec3(xl, yl, zl));
						}
					}
				}
			}
		}

		//find line on both planes, find edge point of both planes, if any edge points lie on the other plane, they intersect

	}
}

void World::add_on_line(int type, vec3 p1, vec3 p2) {

	std::vector<vec3> matches;

	vec3 start = p1;
	vec3 end = p2;
	for (int i = 0; i < 3; i++) {
		int step = 1;
		int startZ = ceil(start[i]);
		if (end[i] < start[i]) {
			step = -1;
			startZ = floor(start[i]);
		}
		while ((end[i] < start[i] && end[i] < startZ) || (end[i] > start[i] && end[i] > startZ)) {
			float dif = startZ - start[i];
			vec3 norm = glm::normalize(end - start);
			float scale = dif / norm[i];
			vec3 res = norm * scale;

			vec3 pos = start + res;
			if (step == -1) {
				pos[i] -= 1;
			}

			chunkController->add_generated_block(type, pos);
			matches.push_back(pos);

			startZ += step;
		}
	}
}

int World::get_height(int x, int y) {
	double changeFactor = .05;
	int scale = 15;
	double val = perlinModule.GetValue(x * changeFactor, y * changeFactor, .50);
	int height = (int)(val * scale);
	return height;
}

bool World::loaded_area_changed(bool should_fill) {

	std::vector<vec3> best_distances;

	//float best_dist = 0;
	//float best_x = 0;
	//float best_y = 0;

	bool has_changed = false;
	int length_side = (std::floor(chunkController->load_distance / CHUNK_SIZE) + 1) * CHUNK_SIZE;
	int nearestChunkX = std::floor(camera->position.x / CHUNK_SIZE) * CHUNK_SIZE;
	int nearestChunkY = std::floor(camera->position.y / CHUNK_SIZE) * CHUNK_SIZE;
	for (int i = nearestChunkX - CHUNK_SIZE - length_side; i <= nearestChunkX + CHUNK_SIZE + length_side; i += CHUNK_SIZE) {
		for (int j = nearestChunkY - CHUNK_SIZE - length_side; j <= nearestChunkY + CHUNK_SIZE + length_side; j += CHUNK_SIZE) {
			float dist = chunkController->flat_dist_camera(i, j);
			if (dist < chunkController->load_distance && !chunkController->has_chunk(i, j)) {

				if (should_fill) {
					int c = 0;
					for (vec3 pos : best_distances) {
						if (dist < pos[2]) {
							break;
						}
						c++;
					}

					if (best_distances.size() > 0) {
						best_distances.push_back(vec3());
						for (int shift = best_distances.size() - 1; shift >= c + 1; shift--) {
							best_distances[shift] = best_distances[shift - 1];
						}
						best_distances[c] = vec3(i, j, dist);
					}
					else {
						best_distances.push_back(vec3(i, j, dist));
					}

					/*if (dist < best_dist || best_dist == 0) {
						best_dist = dist;
						best_x = i;
						best_y = j;
					}*/
					has_changed = true;
				}
				else {
					return true;
				}
			}

		}
	}

	if (should_fill && has_changed) {
		//gen_new_chunks(best_distances[0].x, best_distances[0].y);
		for (vec3 dist : best_distances) {
			gen_new_chunks(dist.x, dist.y);
			/*if (controller->get_dt() < .01) {
				gen_new_chunks(dist.x, dist.y);
			}
			else {
				break;
			}*/
		}
	}

	return has_changed;
}