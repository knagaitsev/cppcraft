#include "GameInstance.h"


GameInstance::GameInstance() {

	Game::shaderManager->add("shader1", "../../../shaders/vertex1.glsl", "../../../shaders/fragment1.glsl");
	Game::shaderManager->add("shader2", "../../../shaders/vertex2.glsl", "../../../shaders/fragment1.glsl");
	Game::shaderManager->add("shader3", "../../../shaders/vertex3.glsl", "../../../shaders/fragment1.glsl");

	Game::textureManager->add("blocks", "../../../asset/gimp/blocks4.png");
	Game::textureManager->add("sample", "../../../asset/sample.png");
	Game::textureManager->add("crosshair", "../../../asset/gimp/crosshair.png");
	Game::textureManager->add("lower_inventory", "../../../asset/gimp/lower_inventory.png");
	Game::textureManager->add("lower_inventory_selector", "../../../asset/gimp/lower_inventory_selector.png");

	camera->attach_programs();

	chunkController = new ChunkController(camera);

	controller = new PlayerController(chunkController);

	world = new World(chunkController, controller);
}

void GameInstance::update() {

	//drawing
	//glDrawElements(GL_TRIANGLES, NELEMS(elements), GL_UNSIGNED_INT, 0);
	chunkController->draw();

	world->update();

	//update controller
	controller->update();
}