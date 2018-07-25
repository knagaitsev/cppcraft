#include "PlayerController.h"

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

PlayerController::PlayerController(ChunkController *chunkController): chunkController(chunkController) {

	window = Game::window;
	camera = Game::camera;

	t_start = std::chrono::high_resolution_clock::now();

	glfwGetCursorPos(window, &mx, &my);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursor_disabled = true;

	glfwSetWindowUserPointer(window, this);

	auto click_func = [](GLFWwindow* window, int button, int action, int mods)
	{
		static_cast<PlayerController*>(glfwGetWindowUserPointer(window))->on_mouse_button(window, button, action, mods);
	};

	glfwSetMouseButtonCallback(window, click_func);

	auto scroll_func = [](GLFWwindow* window, double xoffset, double yoffset)
	{
		static_cast<PlayerController*>(glfwGetWindowUserPointer(window))->on_scroll(window, xoffset, yoffset);
	};

	glfwSetScrollCallback(window, scroll_func);

	auto key_func = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		static_cast<PlayerController*>(glfwGetWindowUserPointer(window))->on_key(window, key, scancode, action, mods);
	};

	glfwSetKeyCallback(window, key_func);
}

void PlayerController::update() {
	//camera->position.z = -10.0f;

	if (cursor_disabled && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursor_disabled = false;
	}

	auto t_now = std::chrono::high_resolution_clock::now();
	//auto t_now = chrono::system_clock::now();
	float dt = get_dt();
	//float dt = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
	t_start = t_now;
	
	float fps = 1.0f / dt;

	//printf("%f\n", fps);

	center = camera->center;
	position = camera->position;
	up = camera->up;

	vec3 dif = center - position;
	vec3 normal = normalize(dif);

	vec3 posResult = position;
	vec3 centerResult = center;

	float mag = speed * dt;

	vec3 motionVec = vec3(0.0f, 0.0f, 0.0f);

	vec3 normalFlat = perp(normal, up);

	int blockX = floor(position.x);
	int blockY = floor(position.y);
	int blockZ = floor(position.z);

	vec3 normalFlatPerp = rotate_vec3(normalFlat, radians(90.0f), up);

	if (glfwGetKey(window, KEY_FORWARD)) {
		motionVec += normalFlat;
	}
	else if (glfwGetKey(window, KEY_BACKWARD)) {
		motionVec -= normalFlat;
	}

	if (glfwGetKey(window, KEY_LEFT)) {
		motionVec += normalFlatPerp;
	}
	else if (glfwGetKey(window, KEY_RIGHT)) {
		motionVec -= normalFlatPerp;
	}

	if (motionVec != vec3(0.0f, 0.0f, 0.0f)) {
		motionVec = normalize(motionVec) * mag;
	}

	if (glfwGetKey(window, KEY_UP)) {
		vec3 move = normalize(up) * mag;
		motionVec += move;
	}
	else if (glfwGetKey(window, KEY_DOWN)) {
		vec3 move = normalize(up) * mag;
		motionVec -= move;
	}

	if (collisions_enabled) {
		vec3 hit_center = vec3(position.x, position.y, position.z - player_head_offset);
		vec3 hit_dim = player_dimensions;
		vec3 good_center = good_hitbox_center(hit_center, hit_dim, motionVec, center - position);
		good_center.z += player_head_offset;
		motionVec = good_center - position;
	}

	position += motionVec;
	center += motionVec;

	handle_mouse_input(dt);

	camera->position = position;
	camera->center = center;
	camera->up = up;

	inventory.update();
}

void PlayerController::handle_mouse_input(double dt) {
	double nx, ny;
	glfwGetCursorPos(window, &nx, &ny);
	double dx = mx - nx;
	double dy = my - ny;
	mx = nx;
	my = ny;

	float angleX = 0.003f * dx;
	float angleY = 0.003f * dy;

	vec3 originalCenter = center;

	rotate_center(angleX, up);

	vec3 dif = center - position;
	vec3 normal = normalize(dif);
	vec3 normalFlat = perp(normal, up);
	vec3 normalFlatPerp = rotate_vec3(normalFlat, radians(90.0f), up);

	float maxAngle = 90.0f;
	float vertAngle = glm::degrees(glm::angle(normal, normalFlat));
	float angleFromUpVector = glm::degrees(glm::angle(normal, up));
	vertAngle *= angleFromUpVector > 90.0f ? -1 : 1;

	if (std::abs(vertAngle + degrees(angleY)) >= maxAngle) {
		float newAngleY = SIGN(vertAngle) * maxAngle - vertAngle;
		rotate_center(-radians(newAngleY), normalFlatPerp);
	}
	else {
		rotate_center(-angleY, normalFlatPerp);
	}
}

void PlayerController::rotate_center(float angle, vec3 axis) {
	vec3 dif = center - position;
	vec3 rot = rotate_vec3(dif, angle, axis);
	center = position + rot;
}

bool PlayerController::test_hitbox(glm::vec3 center, glm::vec3 dimensions) {

	int bounds[6];
	for (int i = 0; i < 3; i++) {
		int low = floor(center[i] - (dimensions[i] / 2.0f));
		int high = floor(center[i] + (dimensions[i] / 2.0f));
		bounds[i * 2] = low;
		bounds[i * 2 + 1] = high;
	}

	for (int x = bounds[0]; x <= bounds[1]; x++) {
		for (int y = bounds[2]; y <= bounds[3]; y++) {
			for (int z = bounds[4]; z <= bounds[5]; z++) {
				int blockType = chunkController->get_block(x, y, z);
				if (blockType != AIR_BLOCK && blockType != WATER_BLOCK) {
					return false;
				}
			}
		}
	}
	
	return true;
}

glm::vec3 PlayerController::good_hitbox_center(glm::vec3 current_center, glm::vec3 dimensions, glm::vec3 motionVec, glm::vec3 look_direction) {

	vec3 result = current_center;

	bool flip_horiz_precedence = false;
	if (ABS(look_direction.y) > ABS(look_direction.x)) {
		flip_horiz_precedence = true;
	}

	for (int index = 0; index < 3; index++) {
		int i = index;
		if (flip_horiz_precedence && i == 0) {
			i = 1;
		}
		else if (flip_horiz_precedence && i == 1) {
			i = 0;
		}
		if (motionVec[i] != 0.0f) {
			vec3 singleAxisMotion = vec3();
			singleAxisMotion[i] = motionVec[i];
			if (test_hitbox(result + singleAxisMotion, dimensions)) {
				result[i] += motionVec[i];
			}
			else if (ABS(motionVec[i]) < 1.0f) {
				//block below
				if (motionVec[i] < 0.0f) {
					int bound = floor(result[i] + motionVec[i] - (dimensions[i] / 2.0f));
					result[i] = (bound + 1) + dimensions[i] / 2.0f + .001f;
				}
				//block above
				else {
					int bound = floor(result[i] + motionVec[i] + (dimensions[i] / 2.0f));
					result[i] = (bound) - dimensions[i] / 2.0f - .001f;
				}
			}
		}
	}

	return result;
}

void PlayerController::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		handle_click(true);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		handle_click(false);
	}

	if (!cursor_disabled) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursor_disabled = true;
	}
}

void PlayerController::handle_click(bool break_block) {

	if (!break_block && inventory.building_block_type == AIR_BLOCK) {
		return;
	}

	vec3 start = camera->position;
	vec3 end = camera->center;
	end = start + 6.0f * glm::normalize(end - start);
	float bestScale = -1.0f;
	vec3 block;
	int direc;
	for (int i = 0; i < 3; i++) {
		int step = 1;
		int startZ = ceil(start[i]);
		if (end[i] < start[i]) {
			step = -1;
			startZ = floor(start[i]);
		}
		while ((end[i] < start[i] && end[i] < startZ) || (end[i] > start[i] && end[i] > startZ)) {
			float dif = startZ - start[i];
			vec3 norm = glm::normalize(end - start);
			float scale = dif / norm[i];
			vec3 res = norm * scale;

			vec3 pos = start + res;
			int b = chunkController->get_block(pos);
			if (step == 1 && b != AIR_BLOCK && b != WATER_BLOCK && (bestScale == -1.0f || scale < bestScale)) {
				block = pos;
				bestScale = scale;
				direc = i;
			}
			pos[i] -= 1;
			b = chunkController->get_block(pos);
			if (step == -1 && b != AIR_BLOCK && b != WATER_BLOCK && (bestScale == -1.0f || scale < bestScale)) {
				block = pos;
				bestScale = scale;
				direc = i;
			}

			startZ += step;
		}
	}

	if (bestScale != -1.0f) {
		if (break_block) {
			chunkController->add_block(AIR_BLOCK, block);
			chunkController->gen_changed_buffers();
		}
		else {

			block[direc] += (end[direc] > start[direc] ? -1 : 1);
			int old_type = chunkController->get_block(block);
			chunkController->add_block(inventory.building_block_type, block);

			vec3 player_center = camera->position;
			player_center.z -= player_head_offset;
			if (!collisions_enabled || test_hitbox(player_center, player_dimensions)) {
				chunkController->gen_changed_buffers();
			}
			else {
				chunkController->add_block(old_type, block);
			}
		}
	}
}

void PlayerController::on_scroll(GLFWwindow* window, double xoffset, double yoffset) {
	inventory.on_scroll(window, xoffset, yoffset);
}

float PlayerController::get_dt() {
	auto t_now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
}

void PlayerController::on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	inventory.on_key(window, key, scancode, action, mods);
}