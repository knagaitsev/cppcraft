#ifndef _Camera_h_
#define _Camera_h_

#include "util.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Shaders/ShaderManager.h"

class Camera {
private:
	GLFWwindow *window;

	glm::mat4 proj;
	glm::mat4 view;

public:
	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

	Camera(GLFWwindow *window, float fovy, float width, float height, float zNear, float zFar);

	void apply_projection(GLint location);

	void update_view(GLint location);

	void update();
};

#endif
