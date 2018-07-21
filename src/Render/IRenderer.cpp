#include "IRenderer.h"

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

using glm::vec3;

void IRenderer::gen_vertices_buffer(std::vector<GLfloat> *vertices) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), vertices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->vertices.clear();
	for (int i = 0; i < vertices->size(); i++) {
		this->vertices.push_back((*vertices)[i]);
	}

	vertices_size = vertices->size();
}

void IRenderer::gen_elements_buffer(std::vector<GLuint> *elements) {

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements->size() * sizeof(GLuint), elements->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->elements.clear();
	for (int i = 0; i < elements->size(); i++) {
		this->elements.push_back((*elements)[i]);
	}
	//std::copy(elements->begin(), elements->end(), this->elements.begin());

	elements_size = elements->size();
}

void IRenderer::bind_texture(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void IRenderer::bind_buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void IRenderer::unbind_buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IRenderer::draw(GLuint program, GLuint texture) {

	bind_texture(texture);
	bind_buffer();

	glUseProgram(program);

	GLint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), 0);

	GLint tex = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(tex);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glDrawElements(GL_TRIANGLES, elements_size, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_size * vertex_size);

	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(tex);

	unbind_buffer();
}

void IRenderer::draw_transparent(GLuint program, GLuint texture, vec3 player_pos) {

	bind_texture(texture);
	bind_buffer();

	glUseProgram(program);

	GLint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), 0);

	GLint tex = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(tex);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((triangles[i].index * 3) * sizeof(GLfloat)));
	}

	//int size = elements_size >= 3 ? 3 : elements_size;
	
	//glDrawArrays(GL_TRIANGLES, 0, vertices_size * vertex_size);

	unbind_buffer();
}

void IRenderer::delete_buffers() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}