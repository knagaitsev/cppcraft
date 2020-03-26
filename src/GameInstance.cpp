#include "GameInstance.h"

#include "Shaders/BlockShader.h"
#include "Shaders/StaticShader.h"

GameInstance::GameInstance() {

	ShaderProgram* block_shader = new BlockShader("../shaders/BlockVertex.glsl", "../shaders/BlockFragment.glsl");
	Game::shaderManager->add("block_shader", block_shader);
	ShaderProgram* perspective_shader = new StaticShader("../shaders/ProjectedVertex.glsl", "../shaders/StaticFragment.glsl");
	Game::shaderManager->add("perspective_shader", perspective_shader);
	ShaderProgram* static_shader = new StaticShader("../shaders/StaticVertex.glsl", "../shaders/StaticFragment.glsl");
	Game::shaderManager->add("static_shader", static_shader);

	Game::textureManager->add("blocks", "../asset/gimp/blocks4.png");
	Game::textureManager->add("sample", "../asset/sample.png");
	Game::textureManager->add("crosshair", "../asset/gimp/crosshair.png");
	Game::textureManager->add("lower_inventory", "../asset/gimp/lower_inventory.png");
	Game::textureManager->add("lower_inventory_selector", "../asset/gimp/lower_inventory_selector.png");

	camera->apply_projection(Game::shaderManager->get("block_shader")->get_proj_location());
	camera->apply_projection(Game::shaderManager->get("perspective_shader")->get_proj_location());

	chunkController = new ChunkController(camera);

	controller = new PlayerController(chunkController);

	world = new World(chunkController, controller);
}

void GameInstance::update() {

	camera->update_view(Game::shaderManager->get("block_shader")->get_view_location());

	//drawing
	//glDrawElements(GL_TRIANGLES, NELEMS(elements), GL_UNSIGNED_INT, 0);
	chunkController->draw();

	world->update();

	//update controller
	controller->update();
}