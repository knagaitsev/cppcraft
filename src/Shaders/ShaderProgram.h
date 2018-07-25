#ifndef _ShaderProgram_h_
#define _ShaderProgram_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

class ShaderProgram {
private:
protected:
	GLuint program;
public:
	ShaderProgram();
	ShaderProgram(const char *vertex_path, const char *fragment_path);
	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual GLint get_proj_location() = 0;
	virtual GLint get_view_location() = 0;
};

#endif