#ifndef _Camera_h_
#define _Camera_h_

#include "util.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Shaders/ShaderManager.h"

class Camera {
private:
	ShaderManager *shaderManager;
	GLFWwindow *window;
	GLint uniView;

	glm::mat4 proj;

public:
	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

	Camera(ShaderManager *shaderManager, GLFWwindow *window, float fovy, float width, float height, float zNear, float zFar);

	void attach_programs();

	void update();
};

#endif
