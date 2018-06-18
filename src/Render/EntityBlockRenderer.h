#ifndef _EntityBlockRenderer_h_
#define _EntityBlockRenderer_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "IRenderer.h"

#include "../util.h"

#include "../World/BlockUtil.h"

class EntityBlockRenderer : public IRenderer {
private:

public:
	EntityBlockRenderer();
	void gen_buffer(int type, float x, float y, float z, float rotationX, float rotationY, float rotationZ, float scale);
	void gen_buffer(int type, glm::vec3 position, glm::vec3 axis_rotation, float scale);
};

#endif