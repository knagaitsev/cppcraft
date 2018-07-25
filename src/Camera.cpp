#include "Camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <array>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

using glm::vec3;

Camera::Camera(ShaderManager *shaderManager, GLFWwindow *window, float fovy, float width, float height, float zNear, float zFar): window(window), shaderManager(shaderManager) {

	float aspectRatio = width / height;
	proj = glm::perspective(glm::radians(fovy), aspectRatio, zNear, zFar);

	position = vec3(-4.0f, 0.0f, 20.0f);
	center = vec3(0.0f, 0.0f, 0.0f);
	up = vec3(0.0f, 0.0f, 1.0f);
}

void Camera::update() {
	glm::mat4 view = glm::lookAt(
		position,
		center,
		up
	);

	glUniformMatrix4fv(shaderManager->get("shader1").get_view_location(), 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::attach_programs() {
	glUniformMatrix4fv(shaderManager->get("shader1").get_proj_location(), 1, GL_FALSE, glm::value_ptr(proj));

	glUniformMatrix4fv(shaderManager->get("shader2").get_proj_location(), 1, GL_FALSE, glm::value_ptr(proj));
}