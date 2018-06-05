#ifndef _ChunkController_h_
#define _ChunkController_h_

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <array>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Chunk.h"

#include "../Render/BlockRenderer.h"

#include "../util.h"

#include "../Camera.h"

#include <json/reader.h>
#include <json/writer.h>

class ChunkController {
private:
	std::vector<Chunk *> chunks;

	Camera *camera;

public:
	float flat_dist_camera(int x, int y);

	const float render_distance = 50.0f;

	const float load_distance = 60.0f;

	const float cut_angle = 55.0f;

	ChunkController(Camera *camera);

	int get_block(int x, int y, int z);
	int get_block(glm::vec3 vec) { return get_block(floor(vec.x), floor(vec.y), floor(vec.z)); };

	void add_block(int type, int x, int y, int z);
	void add_block(int type, glm::vec3 vec) { add_block(type, floor(vec.x), floor(vec.y), floor(vec.z)); };

	void draw(Attrib *attrib);

	void gen_one_buffer();
	void gen_all_buffers();
	void gen_changed_buffers();

	void save_data(std::string filename);
	Json::Value read_data(std::string filename);
	void load_data(std::string filename);

	bool has_chunk(int x, int y);

	void remove_chunks();
};

#endif