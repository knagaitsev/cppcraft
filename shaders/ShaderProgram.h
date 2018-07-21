#ifndef _ShaderProgram_h_
#define _ShaderProgram_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../util.h"

class IRenderer {
private:
	GLuint program;
protected:
	
public:
	ShaderProgram(const char *vertex_path, const char *fragment_path);
	void bind();
	void unbind();
};

#endif