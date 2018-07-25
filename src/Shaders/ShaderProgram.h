#ifndef _ShaderProgram_h_
#define _ShaderProgram_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

class ShaderProgram {
private:
	GLuint program;
	GLint position;
	GLint tex;
protected:
	
public:
	ShaderProgram();
	ShaderProgram(const char *vertex_path, const char *fragment_path);
	void bind();
	void unbind();

	GLint get_proj_location();
	GLint get_view_location();
};

#endif