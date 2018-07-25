#include "InventoryRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;

using namespace glm;
using namespace std;

InventoryRenderer::InventoryRenderer() {
	window = Game::window;
}

void InventoryRenderer::gen_buffer(vec2 tright, vec2 tleft, vec2 bleft, vec2 bright) {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	const int blocks_per_image = 8;

	const GLfloat image_size = (1.0f / blocks_per_image);

	GLfloat tex[4] = {
		0.0f, 1.0f, 0.0f, 1.0f
	};

	/*int faces[6];
	std::copy_n(std::begin(BLOCK_FACES[type]), 6, std::begin(faces));
	for (int i = 0; i < NELEMS(faces); i++) {
		GLfloat texX = (faces[i] % 8) * image_size;
		tex[i * 4] = texX;
		tex[i * 4 + 1] = texX + image_size;

		GLfloat texY = (faces[i] / 8) * image_size;
		tex[i * 4 + 2] = texY;
		tex[i * 4 + 3] = texY + image_size;
	}*/

	GLfloat verticesArray[] = {

		//  Position    Texcoords
		//x    y     z

		//front-back
		tleft.x,  tleft.y, 0.0f, tex[0], tex[2], // Top-right
		tright.x,  tright.y, 0.0f, tex[1], tex[2], // Top-left
		bright.x,  bright.y, 0.0f, tex[1], tex[3],  // Bottom-left
		bleft.x,  bleft.y, 0.0f, tex[0], tex[3], // Bottom-right
	};

	GLuint elementArray[] = {
		0, 1, 2,
		2, 3, 0
	};

	for (int i = 0; i < NELEMS(verticesArray); i++) {
		vertices.push_back(verticesArray[i]);
	}

	for (int i = 0; i < 6; i++) {
		elements.push_back(elementArray[i]);
	}

	gen_vertices_buffer(&vertices);
	gen_elements_buffer(&elements);
}

void InventoryRenderer::gen_buffer(float centerX, float centerY, unsigned int width, unsigned int height) {
	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);
	float left = (float)(centerX - ((float)width / 2)) / ((float)window_width / 2);
	float right = (float)(centerX + ((float)width / 2)) / ((float)window_width / 2);
	float top = (float)(centerY + ((float)height / 2)) / ((float)window_height / 2);
	float bottom = (float)(centerY - ((float)height / 2)) / ((float)window_height / 2);

	gen_buffer(vec2(right, top), vec2(left, top), vec2(left, bottom), vec2(right, bottom));
}

void InventoryRenderer::gen_block_buffer(int type, float centerX, float centerY, unsigned int height) {
	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);

	std::vector<GLfloat> vertices;
	std::vector<GLuint> elements;

	const int blocks_per_image = 8;

	const GLfloat image_size = (1.0f / blocks_per_image);
	int faces[6];
	std::copy_n(std::begin(BLOCK_FACES[type]), 6, std::begin(faces));

	vec2 vmain = vec2(centerX, centerY);
	vec2 vtop = vec2(vmain.x, vmain.y + (float)height / 2);
	vec2 vbottom = vec2(vmain.x, vmain.y - (float)height / 2);

	glm::mat4 trans(1);
	trans = glm::rotate(trans, radians(120.0f), vec3(0, 0, 1));

	vec2 vbleft = vmain + glm::vec2(trans * glm::vec4(vtop - vmain, 0.0f, 1.0f));
	vec2 vbright = vmain + glm::vec2(trans * glm::vec4(vbleft - vmain, 0.0f, 1.0f));

	vec2 vtright = vmain + glm::vec2(trans * glm::vec4(vbottom - vmain, 0.0f, 1.0f));
	vec2 vtleft = vmain + glm::vec2(trans * glm::vec4(vtright - vmain, 0.0f, 1.0f));

	vec2 verts[] = {
		vmain, vtop, vbottom, vbleft, vbright, vtright, vtleft
	};

	for (int i = 0; i < NELEMS(verts); i++) {
		verts[i].x = (float)(verts[i].x) / ((float)window_width / 2);
		verts[i].y = (float)(verts[i].y) / ((float)window_height / 2);
	}

	vmain = verts[0];
	vtop = verts[1];
	vbottom = verts[2];
	vbleft = verts[3];
	vbright = verts[4];
	vtright = verts[5];
	vtleft = verts[6];

	vec2 tleft;
	vec2 tright;
	vec2 bleft;
	vec2 bright;

	int elemCount = 0;
	for (int block = 0; block < 3; block++) {
		if (block == 0) {
			tleft = vmain;
			tright = vtright;
			bleft = vbottom;
			bright = vbright;
		}
		else if (block == 1) {
			tleft = vtleft;
			tright = vmain;
			bleft = vbleft;
			bright = vbottom;
		}
		else {
			tleft = vtop;
			tright = vtright;
			bleft = vtleft;
			bright = vmain;
		}

		GLfloat tex[4] = {
			0.0f, 1.0f, 0.0f, 1.0f
		};

		GLfloat texX = (faces[block * 2] % 8) * image_size;
		tex[0] = texX;
		tex[1] = texX + image_size;

		GLfloat texY = (faces[block * 2] / 8) * image_size;
		tex[2] = texY;
		tex[3] = texY + image_size;

		GLfloat verticesArray[] = {

			//  Position    Texcoords
			//x    y     z

			//front-back
			tleft.x,  tleft.y, 0.0f, tex[0], tex[2], // Top-right
			tright.x,  tright.y, 0.0f, tex[1], tex[2], // Top-left
			bright.x,  bright.y, 0.0f, tex[1], tex[3],  // Bottom-left
			bleft.x,  bleft.y, 0.0f, tex[0], tex[3], // Bottom-right
		};

		GLuint elementArray[] = {
			0, 1, 2,
			2, 3, 0
		};

		for (int i = 0; i < NELEMS(verticesArray); i++) {
			vertices.push_back(verticesArray[i]);
		}

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

void InventoryRenderer::printVec2(glm::vec2 vec) {
	cout << vec.x << ", " << vec.y << endl;
}

void InventoryRenderer::update() {

}