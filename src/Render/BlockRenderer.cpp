#include "BlockRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

BlockRenderer::BlockRenderer() {

}

void BlockRenderer::gen_vertices_buffer(std::vector<GLfloat> *vertices) {

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), vertices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vertices_size = vertices->size();
}

void BlockRenderer::gen_elements_buffer(std::vector<GLuint> *elements) {

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements->size() * sizeof(GLuint), elements->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	elements_size = elements->size();
}

void BlockRenderer::gen_buffer(Block (*blocks)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], int offsetX, int offsetY, int offsetZ, Block(*neighbor_blocks[6])[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
	
	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	int elemCount = 0;

	int size = CHUNK_SIZE;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				Block block = (*blocks)[x][y][z];
				if (block.type != AIR_BLOCK) {


					bool fill_faces[6];
					std::fill(std::begin(fill_faces), std::end(fill_faces), true);

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


						if (neighborType != AIR_BLOCK && !BLOCK_FACES[neighborType][6]) {
							fill_faces[i] = false;
						}

						if (!BLOCK_FACES[neighborType][6]) {
							neighbors_have_transparency[i] = true;
						}

						coords[i / 2] = originalCoord;
					}
					
					/*if ((x + 1 < CHUNK_SIZE && (*blocks)[x + 1][y][z].type != AIR_BLOCK) || (x + 1 == CHUNK_SIZE && neighbor_blocks[0] != nullptr && (*neighbor_blocks[0])[0][y][z].type != AIR_BLOCK)) {
						fill_faces[0] = false;
					}

					if ((x - 1 >= 0 && (*blocks)[x - 1][y][z].type != AIR_BLOCK) || (x == 0 && neighbor_blocks[1] != nullptr && (*neighbor_blocks[1])[CHUNK_SIZE - 1][y][z].type != AIR_BLOCK)) {
						fill_faces[1] = false;
					}

					if ((y + 1 < CHUNK_SIZE && (*blocks)[x][y + 1][z].type != AIR_BLOCK) || (y + 1 == CHUNK_SIZE && neighbor_blocks[2] != nullptr && (*neighbor_blocks[2])[x][0][z].type != AIR_BLOCK)) {
						fill_faces[2] = false;
					}

					if ((y - 1 >= 0 && (*blocks)[x][y - 1][z].type != AIR_BLOCK) || (y == 0 && neighbor_blocks[3] != nullptr && (*neighbor_blocks[3])[x][CHUNK_SIZE - 1][z].type != AIR_BLOCK)) {
						fill_faces[3] = false;
					}

					//block above
					if ((z + 1 < CHUNK_SIZE && (*blocks)[x][y][z + 1].type != AIR_BLOCK) || (z + 1 == CHUNK_SIZE && neighbor_blocks[4] != nullptr && (*neighbor_blocks[4])[x][y][0].type != AIR_BLOCK)) {
						fill_faces[4] = false;
					}

					//block below
					if ((z - 1 >= 0 && (*blocks)[x][y][z - 1].type != AIR_BLOCK) || (z == 0 && neighbor_blocks[5] != nullptr && (*neighbor_blocks[5])[x][y][CHUNK_SIZE - 1].type != AIR_BLOCK)) {
						fill_faces[5] = false;
					}*/

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

					GLfloat verticesArray[] = {

						//  Position    Texcoords
						//x    y     z

						//front-back
						0.5f,  0.5f, 0.5f, tex[0], tex[2], // Top-right
						0.5f,  -0.5f, 0.5f, tex[1], tex[2], // Top-left
						0.5f, -0.5f, -0.5f, tex[1], tex[3],  // Bottom-left
						0.5f, 0.5f, -0.5f, tex[0], tex[3], // Bottom-right

						-0.5f,  -0.5f, 0.5f, tex[0 + 4], tex[2 + 4], // Top-left
						-0.5f,  0.5f, 0.5f, tex[1 + 4], tex[2 + 4], // Top-right
						-0.5f, 0.5f, -0.5f, tex[1 + 4], tex[3 + 4], // Bottom-right
						-0.5f, -0.5f, -0.5f, tex[0 + 4], tex[3 + 4],  // Bottom-left

						//left-right
						-0.5f,  0.5f, 0.5f, tex[0 + 8], tex[2 + 8], // Top-right
						0.5f,  0.5f, 0.5f, tex[1 + 8], tex[2 + 8], // Top-right
						0.5f, 0.5f, -0.5f, tex[1 + 8], tex[3 + 8], // Bottom-right
						-0.5f, 0.5f, -0.5f, tex[0 + 8], tex[3 + 8], // Bottom-right

						0.5f,  -0.5f, 0.5f, tex[0 + 12], tex[2 + 12], // Top-left
						-0.5f,  -0.5f, 0.5f, tex[1 + 12], tex[2 + 12], // Top-left
						-0.5f, -0.5f, -0.5f, tex[1 + 12], tex[3 + 12],  // Bottom-left
						0.5f, -0.5f, -0.5f, tex[0 + 12], tex[3 + 12],  // Bottom-left

						//top-bottom
						- 0.5f,  -0.5f, 0.5f, tex[0 + 16], tex[2 + 16], // Top-left
						0.5f,  -0.5f, 0.5f, tex[1 + 16], tex[2 + 16], // Top-left
						0.5f,  0.5f, 0.5f, tex[1 + 16], tex[3 + 16], // Top-right
						-0.5f,  0.5f, 0.5f, tex[0 + 16], tex[3 + 16], // Top-right

						-0.5f, 0.5f, -0.5f, tex[0 + 20], tex[2 + 20], // Bottom-right
						0.5f, 0.5f, -0.5f, tex[1 + 20], tex[2 + 20], // Bottom-right
						0.5f, -0.5f, -0.5f, tex[1 + 20], tex[3 + 20],  // Bottom-left
						-0.5f, -0.5f, -0.5f, tex[0 + 20], tex[3 + 20]  // Bottom-left
					};

					GLuint elementArray[] = {
						0, 1, 2,
						2, 3, 0
					};

					for (int i = 0; i < NELEMS(verticesArray); i++) {
						if (fill_faces[i / 20]) {
							switch (i % 5) {
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
						}
					}

					for (int i = 0; i < NELEMS(fill_faces); i++) {
						if (fill_faces[i]) {
							for (int reverse = 0; reverse < 2; reverse++) {
								if (reverse == 1 && BLOCK_FACES[block.type][6] == 1) {
									int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;
								}
								else if (reverse != 0) {
									continue;
								}

								for (int j = 0; j < NELEMS(elementArray); j++) {
									elementArray[j] += elemCount;
									elements.push_back(elementArray[j]);
									elementArray[j] -= elemCount;
								}

								if (reverse == 1) {
									int temp = elementArray[0];
									elementArray[0] = elementArray[1];
									elementArray[1] = temp;
									temp = elementArray[3];
									elementArray[3] = elementArray[4];
									elementArray[4] = temp;
								}
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

void BlockRenderer::draw(Attrib *attrib) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


	glEnableVertexAttribArray(attrib->position);
	glVertexAttribPointer(attrib->position, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(attrib->tex);
	glVertexAttribPointer(attrib->tex, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glDrawElements(GL_TRIANGLES, elements_size, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_size * vertex_size);

	glDisableVertexAttribArray(attrib->position);
	glDisableVertexAttribArray(attrib->tex);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}