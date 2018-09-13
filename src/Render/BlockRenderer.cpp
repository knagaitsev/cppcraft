#include "BlockRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

using glm::vec3;

BlockRenderer::BlockRenderer(bool is_water): is_water(is_water) {
	std::vector<GLfloat> v;
	gen_vertices_buffer(&v);

	std::vector<GLuint> e;
	gen_elements_buffer(&e);
}

void BlockRenderer::gen_buffer(Block (*blocks)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], int offsetX, int offsetY, int offsetZ, Block(*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
	
	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	int elemCount = 0;

	GLfloat verticesArray[] = {

		//  Position    Texcoords
		//x    y     z

		//front-back
		0.5f,  0.5f, 0.5f,
		0.5f,  -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		-0.5f,  -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		//left-right
		-0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f,  -0.5f, 0.5f,
		-0.5f,  -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,

		//top-bottom
		-0.5f,  -0.5f, 0.5f,
		0.5f,  -0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f//, tex[0 + 20], tex[3 + 20]
	};

	GLuint elementArray[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint reversedElementArray[] = {
		1, 0, 2,
		3, 2, 0
	};

	int texIndices[] = {
		0, 2,
		1, 2,
		1, 3,
		0, 3
	};

	int size = CHUNK_SIZE;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				Block block = (*blocks)[x][y][z];
				if (block.type != AIR_BLOCK && ((block.type == WATER_BLOCK && is_water) || (block.type != WATER_BLOCK && !is_water))) {

					bool fill_faces[6];
					std::fill(std::begin(fill_faces), std::end(fill_faces), true);

					int neighbor_types[6];
					std::fill(std::begin(neighbor_types), std::end(neighbor_types), AIR_BLOCK);

					bool neighbors_have_transparency[6];
					std::fill(std::begin(neighbors_have_transparency), std::end(neighbors_have_transparency), false);

					int coords[] = {x, y, z};

					for (int i = 0; i < 6; i++) {

						int originalCoord = coords[i / 2];

						int edgeCoord = CHUNK_SIZE - 1;
						int edgeNeighbor = 0;

						if (i % 2 == 0) {
							coords[i / 2] += 1;
						}
						else {
							coords[i / 2] -= 1;

							edgeCoord = 0;
							edgeNeighbor = CHUNK_SIZE - 1;
						}

						int neighborType = AIR_BLOCK;
						if (originalCoord == edgeCoord && neighbor_blocks[i] != nullptr) {
							coords[i / 2] = edgeNeighbor;
							neighborType = (*neighbor_blocks[i])[coords[0]][coords[1]][coords[2]].type;
						}
						else if (coords[i / 2] >= 0 && coords[i / 2] < CHUNK_SIZE) {
							neighborType = (*blocks)[coords[0]][coords[1]][coords[2]].type;
						}

						neighbor_types[i] = neighborType;

						if (neighborType != AIR_BLOCK && (BLOCK_FACES[neighborType][6] == 0 || BLOCK_FACES[block.type][6] == 2)) {
							fill_faces[i] = false;
						}

						if (BLOCK_FACES[neighborType][6] == 1) {
							neighbors_have_transparency[i] = true;
						}

						coords[i / 2] = originalCoord;
					}

					if (is_water && neighbor_types[4] != WATER_BLOCK) {
						fill_faces[4] = true;
					}

					bool one_filled = false;
					for (int f = 0; f < NELEMS(fill_faces); f++) {
						if (fill_faces[f]) {
							one_filled = true;
							break;
						}
					}

					if (!one_filled) {
						continue;
					}

					int faces[6];
					std::copy_n(std::begin(BLOCK_FACES[block.type]), 6, std::begin(faces));

					const int blocks_per_image = 8;

					const GLfloat image_size = (1.0f / blocks_per_image);

					GLfloat tex[24];

					for (int i = 0; i < NELEMS(faces); i++) {
						GLfloat x = (faces[i] % 8) * image_size;
						tex[i * 4] =  x;
						tex[i * 4 + 1] = x + image_size;

						GLfloat y = (faces[i] / 8) * image_size;
						tex[i * 4 + 2] = y;
						tex[i * 4 + 3] = y + image_size;
					}

					//int current_vertex_size = 5;

					//wasteful loop, only should be done if its water
					/*if (is_water) {
						for (int i = 0; i < NELEMS(verticesArray); i++) {
							if (i % current_vertex_size == 2) {
								if (is_water && neighbor_types[4] != WATER_BLOCK && verticesArray[i] == .5f) {
									verticesArray[i] -= .15f;
								}
							}
						}
					}*/

					int vertices_per_face = 4;
					int elems_per_vertex = 3;
					int full_face_count = vertices_per_face * elems_per_vertex;
					for (int i = 0; i < NELEMS(fill_faces); i++) {
						if (fill_faces[i]) {
							int current_vertex = 0;
							for (int j = full_face_count * i; j < full_face_count * (i+1); j++) {
								GLfloat elem = verticesArray[j];
								switch (j % 3) {
								case 0:
									elem += .5f + x + offsetX;
									break;
								case 1:
									elem += .5f + y + offsetY;
									break;
								case 2:
									if (is_water && neighbor_types[4] != WATER_BLOCK && elem == .5f) {
										elem -= .15f;
									}
									elem += .5f + z + offsetZ;
									break;
								}
								vertices.push_back(elem);

								if (j % 3 == 3 - 1) {
									int texOffset = (i * vertices_per_face);
									vertices.push_back(tex[texIndices[current_vertex * 2] + texOffset]);
									vertices.push_back(tex[texIndices[current_vertex * 2 + 1] + texOffset]);

									vec3 normal = get_normal(i);
									for (int k = 0; k < 3; k++) {
										vertices.push_back(normal[k]);
									}

									current_vertex++;
								}
							}
						}
					}

					//wasteful loop, should only look at elements that are filled faces
					/*for (int i = 0; i < NELEMS(verticesArray); i++) {
						int face_index = i / (current_vertex_size * 4);
						if (fill_faces[face_index]) {
							switch (i % current_vertex_size) {
							case 0:
								verticesArray[i] += .5f + x + offsetX;
								break;
							case 1:
								verticesArray[i] += .5f + y + offsetY;
								break;
							case 2:
								verticesArray[i] += .5f + z + offsetZ;
								break;
							}
							vertices.push_back(verticesArray[i]);

							if (i % current_vertex_size == current_vertex_size - 1) {
								vec3 normal = get_normal(face_index);
								for (int j = 0; j < 3; j++) {
									vertices.push_back(normal[j]);
								}
							}
						}
					}*/

					//explain how this works
					for (int i = 0; i < NELEMS(fill_faces); i++) {
						if (fill_faces[i]) {
							for (int reverse = 0; reverse < 2; reverse++) {
								//if the element is being reversed, make the element array
								//counter clockwise on the second go
								if (reverse == 1 && (block.type == WATER_BLOCK || (BLOCK_FACES[block.type][6] == 1 && !neighbors_have_transparency[i]))) {
									/*int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;*/
								}
								else if (reverse != 0) {
									//if the element is not being reversed, skip the second go
									continue;
								}

								//add the current offset of the element array
								for (int j = 0; j < NELEMS(elementArray); j++) {
									//elementArray[j] += elemCount;
									if (reverse == 0) {
										elements.push_back(elementArray[j] + elemCount);
									}
									else if (reverse == 1) {
										elements.push_back(reversedElementArray[j] + elemCount);
									}
									//elementArray[j] -= elemCount;
								}

								//if the element was reversed, switch it back so it doesnt cause problems
								/*if (reverse == 1) {
									int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;
								}*/
							}

							elemCount += 4;
						}
					}
				}
				else {

				}
			}
		}
	}

	gen_vertices_buffer(&vertices);
	gen_elements_buffer(&elements);
}

void BlockRenderer::draw_transparent(std::string programKey, std::string textureKey, vec3 player_pos) {
	begin(programKey, textureKey);

	std::vector<Triangle> triangles;

	for (int i = 0; i < elements_size; i += 6) {
		int triangle_index = (i / 3);
		int vert_index = (elements[i]) * vertex_size;
		int vert_index_2 = (elements[i + 2]) * vertex_size;
		//glm::vec3 p1 = glm::vec3(vertices[vert_index], vertices[vert_index + 1], vertices[vert_index + 2]);
		float x = (vertices[vert_index] + vertices[vert_index_2]) / 2;
		float y = (vertices[vert_index + 1] + vertices[vert_index_2 + 1]) / 2;
		float z = (vertices[vert_index + 2] + vertices[vert_index_2 + 2]) / 2;
		glm::vec3 p1 = glm::vec3(x, y, z);
		float dist = glm::distance2(p1, player_pos);
		Triangle t = { triangle_index, dist };
		triangles.push_back(t);
	}

	std::sort(triangles.begin(), triangles.end(), [](const Triangle &t1, const Triangle &t2) {
		return t1.distance > t2.distance;
	});

	for (int i = 0; i < triangles.size(); i++) {
		draw_triangles(triangles[i].index * 3, 6);
	}

	end(programKey);
}

glm::vec3 BlockRenderer::get_normal(int index) {
	if (index == 0) {
		return vec3(1, 0, 0);
	}
	else if (index == 1) {
		return vec3(-1, 0, 0);
	}
	else if (index == 2) {
		return vec3(0, 1, 0);
	}
	else if (index == 3) {
		return vec3(0, -1, 0);
	}
	else if (index == 4) {
		return vec3(0, 0, 1);
	}
	else if (index == 5) {
		return vec3(0, 0, -1);
	}

	return vec3(1, 0, 0);
}