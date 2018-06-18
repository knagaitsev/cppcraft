#ifndef _InventoryRenderer_h_
#define _InventoryRenderer_h_

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "IRenderer.h"

#include "../util.h"

#include "../World/BlockUtil.h"

using glm::vec2;

class InventoryRenderer : public IRenderer {
private:
	GLFWwindow *window;
	void printVec2(glm::vec2 vec);
public:
	InventoryRenderer(GLFWwindow *window);
	void gen_buffer(vec2 tright, vec2 tleft, vec2 bleft, vec2 bright);
	void gen_buffer(float centerX, float centerY, unsigned int width, unsigned int height);

	void gen_block_buffer(int type, float centerX, float centerY, unsigned int height);
};

#endif