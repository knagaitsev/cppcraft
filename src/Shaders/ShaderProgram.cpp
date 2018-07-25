#include "ShaderProgram.h"

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>

#include "../util.h"

ShaderProgram::ShaderProgram() {

}

ShaderProgram::ShaderProgram(const char *vertex_path, const char *fragment_path) {
	program = load_shader(vertex_path, fragment_path);
	position = glGetAttribLocation(program, "position");
	tex = glGetAttribLocation(program, "texcoord");
}

void ShaderProgram::bind() {
	glUseProgram(program);

	int size = 5;

	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(tex);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void ShaderProgram::unbind() {
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(tex);
}

GLint ShaderProgram::get_proj_location() {
	glUseProgram(program);
	return glGetUniformLocation(program, "proj");
}

GLint ShaderProgram::get_view_location() {
	glUseProgram(program);
	return glGetUniformLocation(program, "view");
}