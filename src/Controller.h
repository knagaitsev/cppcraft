#ifndef _Controller_h_
#define _Controller_h_

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "config.h"

#include "Camera.h"

#include "World/ChunkController.h"
#include "World/BlockUtil.h"

#include "Inventory.h"

class Controller {
private:
	GLFWwindow * window;
	Camera *camera;
	ChunkController *chunkController;

	bool cursor_disabled;

	std::chrono::time_point<std::chrono::steady_clock> t_start;
	const float speed = 10.0f;

	const glm::vec3 player_dimensions = glm::vec3(.5f, .5f, 1.99f);
	const float player_head_offset = 0.5f;

	const bool collisions_enabled = true;

	Inventory inventory;

	glm::vec3 center;
	glm::vec3 position;
	glm::vec3 up;

	double mx;
	double my;
	void handle_mouse_input(double dt);
	void rotate_center(float angle, glm::vec3 axis);

	bool test_hitbox(glm::vec3 center, glm::vec3 dimensions);

	glm::vec3 good_hitbox_center(glm::vec3 current_center, glm::vec3 dimensions, glm::vec3 motionVec, glm::vec3 look_direction);

	void on_mouse_button(GLFWwindow *window, int button, int action, int mods);
	void handle_click(bool break_block);

	void on_scroll(GLFWwindow* window, double xoffset, double yoffset);

	void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Controller(GLFWwindow *window, Camera *camera, ChunkController *chunkController);
	void update(Attrib *attrib);

	float get_dt();
};

#endif