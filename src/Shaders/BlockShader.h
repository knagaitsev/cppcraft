#ifndef _BlockShader_h_
#define _BlockShader_h_

#include <vector>

#include <GL/glew.h>

#include "StaticShader.h"

class BlockShader : public StaticShader {
private:
	GLint position;
	GLint tex;
	GLint normal;
protected:

public:
	BlockShader(const char *vertex_path, const char *fragment_path);
	void bind();
	void unbind();
};

#endif