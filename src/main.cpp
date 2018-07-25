#include "Game.h";
#include "GameInstance.h";

/*
chunk class: 16x16x16 chunks with structs of blocks that contain block type (int)
chunk controller class: contains vector of all the chunks
- could be called world
bluck util class: contains definitions of all the different types of blocks
- could be called block config

Engine Directory:


camera class: handle the projection stuff and call camera.update each time
have a movement/controls class that the camera class calls to handle mouse movement and stuff

main renderer class: 
block renderer class: contains the block vertices array, generates buffer, communicates with the chunk controller

*/

int main() {
	GameInstance::initialize();
	GameInstance game;

	game.start();

	return 0;
}
