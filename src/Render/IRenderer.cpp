#include "IRenderer.h"

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>

#include "../Shaders/ShaderManager.h"

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

void IRenderer::bind_texture(std::string texture) {
	glBindTexture(GL_TEXTURE_2D, Game::textureManager->get(texture));
}

void IRenderer::bind_buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void IRenderer::unbind_buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IRenderer::bind_program(std::string key) {
	ShaderProgram* program = Game::shaderManager->get(key);
	program->bind();
}

void IRenderer::unbind_program(std::string key) {
	ShaderProgram* program = Game::shaderManager->get(key);
	program->unbind();
}

void IRenderer::draw(std::string programKey, std::string textureKey) {

	begin(programKey, textureKey);

	draw_triangles(0, elements_size);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_size * vertex_size);

	end(programKey);
}

void IRenderer::delete_buffers() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void IRenderer::draw_triangles(int start, int length) {
	glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, (void*)((start) * sizeof(GLfloat)));
}

void IRenderer::update() {

}

void IRenderer::begin(std::string programKey, std::string textureKey) {
	bind_texture(textureKey);
	bind_buffer();
	bind_program(programKey);
}

void IRenderer::end(std::string programKey) {
	unbind_program(programKey);
	unbind_buffer();
}