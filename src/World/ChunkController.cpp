#include "ChunkController.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>

#include "BlockUtil.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

using glm::vec3;

using namespace std;
using namespace glm;

ChunkController::ChunkController(Camera *camera): camera(camera) {

	
}

void ChunkController::draw(Attrib *attrib) {
	for (Chunk *c : chunks) {
		glm::vec3 chunk_pos = c->position();
		float dist = flat_dist_camera(chunk_pos.x, chunk_pos.y);

		vec3 look_direc = camera->center - camera->position;
		look_direc.z = 0;

		chunk_pos.x += CHUNK_SIZE / 2;
		chunk_pos.y += CHUNK_SIZE / 2;
		vec3 chunk_direc = chunk_pos - (camera->position - (20.0f * normalize(look_direc)));
		chunk_direc.z = 0;

		if (dist < render_distance/* && (camera->center.z < camera->position.z || (glm::angle(normalize(chunk_direc), normalize(look_direc))) <= radians(cut_angle / 2.0f))*/) {
			c->draw(attrib);
		}
	}


}

int ChunkController::get_block(int x, int y, int z) {
	for (Chunk *c : chunks) {
		if (c->has_block(x, y, z)) {
			return c->get_block(x, y, z).type;
		}
	}

	return 0;
}

//returns the index of the chunk that the block was added to
int ChunkController::add_block(int type, int x, int y, int z) {

	for (int i = 0; i < chunks.size(); i++) {
		if (chunks[i]->add_block(type, x, y, z)) {
			return i;
		}
	}

	int chunkX = CHUNK_SIZE * ((x+(x < -1 ? 1 : 0)) / CHUNK_SIZE);
	int chunkY = CHUNK_SIZE * ((y+(y < -1 ? 1 : 0)) / CHUNK_SIZE);
	int chunkZ = CHUNK_SIZE * ((z+(z < -1 ? 1 : 0)) / CHUNK_SIZE);

	chunkX -= x >= 0 ? 0 : CHUNK_SIZE;
	chunkY -= y >= 0 ? 0 : CHUNK_SIZE;
	chunkZ -= z >= 0 ? 0 : CHUNK_SIZE;

	Chunk *chunk = new Chunk(chunkX, chunkY, chunkZ);
	chunk->add_block(type, x, y, z);

	chunks.push_back(chunk);

	for (int i = 0; i < chunks.size(); i++) {
		chunks[chunks.size() - 1]->add_neighbor(chunks[i]);
		chunks[i]->add_neighbor(chunks[chunks.size() - 1]);
	}

	return chunks.size() - 1;
}

int ChunkController::add_generated_block(int type, int x, int y, int z) {
	int index = add_block(type, x, y, z);
	chunks[index]->set_freshly_generated(true);
	return index;
}

void ChunkController::gen_one_buffer() {
	if (chunks.size() > 0) {
		chunks[0]->gen_buffer();
	}
}

void ChunkController::gen_all_buffers() {
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i]->gen_buffer();
	}
}

void ChunkController::gen_changed_buffers() {
	for (int i = 0; i < chunks.size(); i++) {
		if (chunks[i]->changed()) {
			chunks[i]->gen_buffer();
		}
	}
}

void ChunkController::save_data(std::string filename) {
	/*
	std::ofstream file;
	file.open(filename);
	//read currently saved chunks
	Json::Value list = read_data(filename);
	for (Chunk *chunk : chunks) {
		Json::Value elem;
		glm::vec3 pos = chunk->position();

		bool exists = false;
		int chunkIndex;
		//check if the chunk we are trying to save is already saved
		for (chunkIndex = 0; chunkIndex < list.size(); chunkIndex++) {
			Json::Value chunk = list[chunkIndex];
			int chunkX = chunk["x"].asInt();
			int chunkY = chunk["y"].asInt();
			int chunkZ = chunk["z"].asInt();

			if ((int)pos.x == chunkX && (int)pos.y == chunkY && (int)pos.z == chunkZ) {
				exists = true;
				break;
			}
		}

		elem["x"] = Json::Value((int)pos.x);
		elem["y"] = Json::Value((int)pos.y);
		elem["z"] = Json::Value((int)pos.z);
		
		Json::Value array1(Json::arrayValue);
		for (int i = 0; i < CHUNK_SIZE; i++) {
			Json::Value array2(Json::arrayValue);
			for (int j = 0; j < CHUNK_SIZE; j++) {
				Json::Value array3(Json::arrayValue);
				for (int k = 0; k < CHUNK_SIZE; k++) {
					Block block = chunk->get_block(i, j, k);
					array3.append(Json::Value(block.type));
				}
				array2.append(array3);
			}
			array1.append(array2);
		}
		elem["blocks"] = array1;
		//replace elem if it exists already
		if (exists) {
			list[chunkIndex] = elem;
		}
		else {
			//append new elem if it doesn't exist
			list.append(elem);
		}
	}
	Json::StyledWriter styledWriter;
	file << styledWriter.write(list);

	file.close();
	*/

	ofstream file;
	file.open(filename, ios::trunc);

	if (file.is_open())
	{
		for (Chunk *chunk : chunks) {
			for (int i = 0; i < chunks.size(); i++) {
				if ((int)chunks[i]->position().x == (int)chunk->position().x && (int)chunks[i]->position().y == (int)chunk->position().y) {
					if (!chunks[i]->get_freshly_generated() || !chunk->get_freshly_generated()) {
						chunk->set_freshly_generated(false);
						chunks[i]->set_freshly_generated(false);
					}
				}
			}
		}

		for (Chunk *chunk : chunks) {
			if (!chunk->get_freshly_generated()) {

				glm::vec3 pos = chunk->position();
				file << (int)(pos.x) << "," << (int)(pos.y) << "," << (int)(pos.z) << ",\n";
				for (int i = 0; i < CHUNK_SIZE; i++) {
					for (int j = 0; j < CHUNK_SIZE; j++) {
						for (int k = 0; k < CHUNK_SIZE; k++) {
							file << chunk->get_block(i, j, k).type << ",";
						}
						file << "\n";
					}
					//file << "\n";
				}
			}
		}
		file.close();
	}
	else cout << "Unable to open file: " << filename;
}

Json::Value ChunkController::read_data(std::string filename) {
	std::ifstream file(filename);
	std::string line;
	std::string str = "";
	if (file.good()) {
		if (file.is_open()) {
			while (std::getline(file, line)) {
				str += line;
			}
			file.close();
		}
	}
	else {
		std::cout << "File not found: " << filename << std::endl;
		Json::Value arr(Json::arrayValue);
		return arr;
	}

	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(str, root);
	if (!parsingSuccessful)
	{
		std::cout << "Error Parsing values: " << filename << std::endl;
		Json::Value arr(Json::arrayValue);
		return arr;
	}

	return root;
}

void ChunkController::load_data(std::string filename) {

	std::ifstream file(filename);
	std::string line;
	std::string str = "";
	if (file.good()) {
		if (file.is_open()) {
			int lineIndex = 0;
			int chunkCoords[3];
			while (std::getline(file, line)) {
				std::stringstream ss(line);
				if (lineIndex == 0) {
					int i;
					int count = 0;
					while (ss >> i && count < 3)
					{
						chunkCoords[count] = i;
						count++;
						if (ss.peek() == ',')
							ss.ignore();
					}
				}
				else {
					int i;
					int count = 0;
					while (ss >> i && count < CHUNK_SIZE)
					{
						add_block(i, chunkCoords[0] + ((lineIndex - 1) / CHUNK_SIZE), chunkCoords[1] + ((lineIndex - 1) % CHUNK_SIZE), chunkCoords[2] + count);
						count++;
						if (ss.peek() == ',')
							ss.ignore();
					}
				}

				if (lineIndex == CHUNK_SIZE * CHUNK_SIZE) {
					lineIndex = 0;
				}
				else {
					lineIndex++;
				}
			}
			file.close();
		}
	}
	else {
		std::cout << "File not found: " << filename << std::endl;
	}

	/*
	Json::Value chunk_data = read_data(filename);
	for (int chunkIndex = 0; chunkIndex < chunk_data.size(); chunkIndex++) {
		Json::Value chunk = chunk_data[chunkIndex];
		int chunkX = chunk["x"].asInt();
		int chunkY = chunk["y"].asInt();
		int chunkZ = chunk["z"].asInt();

		Json::Value blocks = chunk["blocks"];

		bool chunk_already_present = false;
		for (Chunk *c : chunks) {
			glm::vec3 pos = c->position();
			if (pos.x == chunkX && pos.y == chunkY && pos.z == chunkZ) {
				chunk_already_present = true;
				break;
			}
		}

		if (!chunk_already_present
			//&& flat_dist_camera(chunkX, chunkY) < load_distance
			) {
			for (int i = 0; i < blocks.size(); i++) {
				for (int j = 0; j < blocks[i].size(); j++) {
					for (int k = 0; k < blocks[i][j].size(); k++) {
						int type = blocks[i][j][k].asInt();
						add_block(type, i + chunkX, j + chunkY, k + chunkZ);
					}
				}
			}
		}
	}
	*/
}

float ChunkController::flat_dist_camera(int x, int y) {
	glm::vec3 chunk_pos = glm::vec3(x, y, 0);
	glm::vec3 cam_pos = camera->position;
	chunk_pos.x += CHUNK_SIZE / 2;
	chunk_pos.y += CHUNK_SIZE / 2;
	cam_pos.z = 0;
	return glm::distance(chunk_pos, cam_pos);
}

bool ChunkController::has_chunk(int x, int y) {
	for (Chunk *chunk : chunks) {
		glm::vec3 pos = chunk->position();
		if (x == pos.x && y == pos.y) {
			return true;
		}
	}
	return false;
}

void ChunkController::remove_chunks() {
	for (int i = chunks.size() - 1; i >= 0; i--) {
		glm::vec3 pos = chunks[i]->position();
		if (flat_dist_camera(pos.x, pos.y) >= load_distance) {
			//delete chunks[i];
			chunks.erase(chunks.begin()+i);
		}
	}
}