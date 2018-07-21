#include "ShaderProgram.h"

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>

ShaderProgram::ShaderProgram(const char *vertex_path, const char *fragment_path) {
	program = load_shader(vertex_path, fragment_path);
}

void ShaderProgram::bind() {
	glUseProgram(program);

	int size = 5;

	GLint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), 0);

	GLint tex = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(tex);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void ShaderProgram::unbind() {
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(tex);
}