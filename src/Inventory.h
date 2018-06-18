#ifndef _Inventory_h_
#define _Inventory_h_

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

#include "Render/EntityBlockRenderer.h"
#include "Render/InventoryRenderer.h"

class Inventory {
private:
	GLFWwindow *window;
	Camera *camera;

	int current_slot = 0;

	glm::vec3 hand_block_pos;
	glm::vec3 hand_block_rotation;
	float hand_block_scale;
	EntityBlockRenderer hand_block;

	std::vector<InventoryRenderer> blocks;

	std::vector<InventoryRenderer> lower_inventory_selectors;

	InventoryRenderer crosshair;

	InventoryRenderer lower_inventory;

public:
	Inventory(GLFWwindow *window, Camera *camera);
	void update(Attrib *attrib);

	int building_block_type = AIR_BLOCK;

	void on_scroll(GLFWwindow* window, double xoffset, double yoffset);
	void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif