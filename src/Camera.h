#ifndef _Camera_h_
#define _Camera_h_

#include "util.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Camera {
private:
	GLFWwindow *window;
	GLint uniView;

public:
	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

	Camera(GLFWwindow *window, Attrib *attrib, float fovy, float width, float height, float zNear, float zFar);

	void update();
};

#endif
