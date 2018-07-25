#include "StaticShader.h"

StaticShader::StaticShader(const char *vertex_path, const char *fragment_path) : ShaderProgram(vertex_path, fragment_path) {
	position = glGetAttribLocation(program, "position");
	tex = glGetAttribLocation(program, "texcoord");
}

void StaticShader::bind() {
	glUseProgram(program);

	int size = 5;

	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(tex);
	glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void StaticShader::unbind() {
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(tex);
}

GLint StaticShader::get_proj_location() {
	glUseProgram(program);
	return glGetUniformLocation(program, "proj");
}

GLint StaticShader::get_view_location() {
	glUseProgram(program);
	return glGetUniformLocation(program, "view");
}