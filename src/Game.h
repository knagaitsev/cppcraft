#ifndef _Game_h_
#define _Game_h_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.h"

#include "Camera.h"

#include "Shaders/ShaderManager.h"
#include "Texture/TextureManager.h"

class Game {
private:

protected:
	static GLFWwindow *window;
	static Camera *camera;
	static ShaderManager *shaderManager;
	static TextureManager *textureManager;

public:
	static void initialize();
	virtual void update() = 0;
	void start();
};

#endif