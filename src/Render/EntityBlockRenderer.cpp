#include "EntityBlockRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

using glm::vec3;

using namespace glm;

EntityBlockRenderer::EntityBlockRenderer() {

}

void EntityBlockRenderer::gen_buffer(int type, float x, float y, float z, float rotationX, float rotationY, float rotationZ, float scale) {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	if (type == AIR_BLOCK) {
		gen_vertices_buffer(&vertices);
		gen_elements_buffer(&elements);
		return;
	}

	const int blocks_per_image = 8;

	const GLfloat image_size = (1.0f / blocks_per_image);

	GLfloat tex[24];

	int faces[6];
	std::copy_n(std::begin(BLOCK_FACES[type]), 6, std::begin(faces));
	for (int i = 0; i < NELEMS(faces); i++) {
		GLfloat texX = (faces[i] % 8) * image_size;
		tex[i * 4] =  texX;
		tex[i * 4 + 1] = texX + image_size;

		GLfloat texY = (faces[i] / 8) * image_size;
		tex[i * 4 + 2] = texY;
		tex[i * 4 + 3] = texY + image_size;
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

	//rotate
	for (int i = 0; i < NELEMS(verticesArray); i++) {
		if (i % vertex_size == 0) {
			vec3 current = vec3(verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]);
			vec3 resZ = rotate_vec3(normalize(vec3(current.x, current.y, 0)), rotationZ, vec3(0, 0, 1));
			current.x = resZ.x;
			current.y = resZ.y;
			vec3 rotatedXAxis = normalize(rotate_vec3(vec3(1, 0, 0), rotationZ, vec3(0, 0, 1)));
			vec3 rotatedYAxis = normalize(rotate_vec3(vec3(0, 1, 0), rotationZ, vec3(0, 0, 1)));

			vec3 resY = rotate_vec3(normalize(current), rotationY, vec3(0, 1, 0));
			current = resY;

			vec3 resX = rotate_vec3(normalize(current), rotationX, vec3(1, 0, 0));
			current = resX;
			verticesArray[i] = current.x;
			verticesArray[i + 1] = current.y;
			verticesArray[i + 2] = current.z;
		}
	}

	for (int i = 0; i < NELEMS(verticesArray); i++) {
		switch (i % vertex_size) {
		case 0:
			verticesArray[i] *= scale;
			verticesArray[i] += x;
			break;
		case 1:
			verticesArray[i] *= scale;
			verticesArray[i] += y;
			break;
		case 2:
			verticesArray[i] *= scale;
			verticesArray[i] += z;
			break;
		}
	}

	int count = 0;
	int used_indices[] = { -1, -1, -1, -1, -1, -1 };
	int max_index = -1;
	float max = 0;
	while (count < 6) {
		for (int i = 0; i < NELEMS(verticesArray); i += vertex_size * 4) {
			bool already_used = false;
			for (int index : used_indices) {
				if (index == i) {
					already_used = true;
					break;
				}
			}
			if (already_used) {
				continue;
			}

			vec3 p1 = vec3(verticesArray[i], verticesArray[i + 1], verticesArray[i + 2]);
			vec3 p2 = vec3(verticesArray[i + vertex_size * 2], verticesArray[i + vertex_size * 2 + 1], verticesArray[i + vertex_size * 2 + 2]);
			vec3 mid = (p1 + p2) / 2.0f;
			float mag = mid.x * mid.x + mid.y * mid.y + mid.z * mid.z;
			if (mag > max) {
				max = mag;
				max_index = i;
			}
		}

		for (int i = max_index; i < max_index + vertex_size * 4; i++) {
			vertices.push_back(verticesArray[i]);
		}
		used_indices[count] = max_index;
		max = 0;
		max_index = -1;

		count++;
	}

	int elemCount = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < NELEMS(elementArray); j++) {
			elementArray[j] += elemCount;
			elements.push_back(elementArray[j]);
			elementArray[j] -= elemCount;
		}
		elemCount += 4;
	}

	gen_vertices_buffer(&vertices);
	gen_elements_buffer(&elements);
}

void EntityBlockRenderer::gen_buffer(int type, glm::vec3 position, glm::vec3 axis_rotation, float scale) {
	gen_buffer(type, position.x, position.y, position.z, axis_rotation.x, axis_rotation.y, axis_rotation.z, scale);
}