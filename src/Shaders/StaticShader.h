#ifndef _StaticShader_h_
#define _StaticShader_h_

#include <vector>

#include <GL/glew.h>

#include "ShaderProgram.h"

class StaticShader : public ShaderProgram {
private:
	GLint position;
	GLint tex;
protected:

public:
	StaticShader(const char *vertex_path, const char *fragment_path);
	void bind();
	void unbind();

	GLint get_proj_location();
	GLint get_view_location();
};

#endif