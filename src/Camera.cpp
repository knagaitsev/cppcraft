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

Camera::Camera(GLFWwindow *window, Attrib *attrib, float fovy, float width, float height, float zNear, float zFar): window(window) {

	glUseProgram(attrib->program);
	uniView = glGetUniformLocation(attrib->program, "view");

	float aspectRatio = width / height;
	glm::mat4 proj = glm::perspective(glm::radians(fovy), aspectRatio, zNear, zFar);
	GLint uniProj = glGetUniformLocation(attrib->program, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	glUseProgram(attrib->program2);
	GLint uniProj2 = glGetUniformLocation(attrib->program2, "proj");
	glUniformMatrix4fv(uniProj2, 1, GL_FALSE, glm::value_ptr(proj));

	position = vec3(-4.0f, 0.0f, 20.0f);
	center = vec3(0.0f, 0.0f, 0.0f);
	up = vec3(0.0f, 0.0f, 1.0f);
}

void Camera::update(Attrib *attrib) {
	glm::mat4 view = glm::lookAt(
		position,
		center,
		up
	);
	glUseProgram(attrib->program);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}