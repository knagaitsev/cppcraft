#include "IRenderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

void IRenderer::gen_vertices_buffer(std::vector<GLfloat> *vertices) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), vertices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vertices_size = vertices->size();
}

void IRenderer::gen_elements_buffer(std::vector<GLuint> *elements) {

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements->size() * sizeof(GLuint), elements->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	elements_size = elements->size();
}

void IRenderer::draw(GLuint program, GLuint texture) {

	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}