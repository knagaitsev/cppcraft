#include "Inventory.h"

#include <math.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/perpendicular.hpp>

#include "util.h"

using glm::vec3;

using namespace glm;
using namespace std;

Inventory::Inventory() {

	window = Game::window;
	camera = Game::camera;

	current_slot = 0;

	building_block_type = current_slot;

	hand_block_pos = vec3(3.5f, -2.0f, -5.0f);
	hand_block_rotation = vec3(radians(280.0f), radians(0.0f), radians(45.0f));
	hand_block_scale = 1.8f;
	hand_block.gen_buffer(building_block_type, hand_block_pos, hand_block_rotation, hand_block_scale);

	crosshair.gen_buffer(0, 0, 25, 25);

	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);
	const int barrier_width = 4;
	const int main_width = 16;
	const int cell_count = 9;
	const int scale = 3;
	int inv_width = scale * (barrier_width * (cell_count + 1) + main_width * cell_count);
	int inv_height = scale * (main_width + barrier_width * 2);
	int inv_middle = -(window_height / 2) + (inv_height / 2);
	lower_inventory.gen_buffer(0, inv_middle, inv_width, inv_height);

	int single_cell_size = scale * (barrier_width + main_width);
	float left = -((float)inv_width / 2) + (((float)single_cell_size) / 2);
	for (int i = 0; i < 9; i++) {
		float centerX = left + single_cell_size * i + 6;
		int centerY = inv_middle + 2;
		InventoryRenderer block;
		if (i > 0) {
			block.gen_block_buffer(i, centerX, centerY, 48);
		}
		blocks.push_back(block);

		InventoryRenderer selec;
		selec.gen_buffer(centerX, centerY, 24 * 3, 25 * 3);
		lower_inventory_selectors.push_back(selec);
	}
}

void Inventory::update() {

	if (glfwGetKey(window, KEY_FORWARD)) {
		
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	hand_block.draw("shader2", "blocks");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	lower_inventory.draw("shader3", "lower_inventory");

	lower_inventory_selectors[current_slot].draw("shader3", "lower_inventory_selector");

	glDisable(GL_BLEND);

	crosshair.draw("shader3", "crosshair");

	for (InventoryRenderer r : blocks) {
		r.draw("shader3", "blocks");
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Inventory::on_scroll(GLFWwindow* window, double xoffset, double yoffset) {


	if (yoffset > 0) {
		if (current_slot == 0) {
			current_slot = 8;
		}
		else {
			current_slot--;
		}
	}
	else if (yoffset < 0) {
		if (current_slot == 8) {
			current_slot = 0;
		}
		else {
			current_slot++;
		}
	}

	building_block_type = current_slot;

	hand_block.gen_buffer(building_block_type, hand_block_pos, hand_block_rotation, hand_block_scale);
}

void Inventory::on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	for (int i = 49; i <= 57; i++) {
		if (key == i && action == GLFW_PRESS) {
			current_slot = i - 49;
			building_block_type = current_slot;
			hand_block.gen_buffer(building_block_type, hand_block_pos, hand_block_rotation, hand_block_scale);
		}
	}
}