#include "Game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

GLFWwindow *Game::window = nullptr;
Camera *Game::camera = nullptr;
ShaderManager *Game::shaderManager = nullptr;
TextureManager *Game::textureManager = nullptr;

void Game::initialize() {

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		//return -1;
	}

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	int xpos = 50;
	int ypos = 50;
	if (monitorCount >= 2) {
		//xpos = -1300;
		xpos = 1800;
		ypos = -150;
	}
	int width = 1280;
	int height = 720;
	window = glfwCreateWindow(width, height, "cppcraft - Loonride", NULL, NULL);
	glfwSetWindowMonitor(window, NULL, xpos, ypos, width, height, 0);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		//return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		//return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
	//  Position    Texcoords
	//x    y     z
	0.0f,  -0.5f, 0.5f, 0.0f, 0.0f, // Top-left
	0.0f,  0.5f, 0.5f, 1.0f, 0.0f, // Top-right
	0.0f, 0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
	0.0f, -0.5f, -0.5f, 0.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
	0, 1, 2,
	2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);*/

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.3f, 0.77f, 1.0f, 0.0f);

	Game::window = window;

	/*
	glEnableVertexAttribArray(attrib.position);
	glVertexAttribPointer(attrib.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(attrib.tex);
	glVertexAttribPointer(attrib.tex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	*/

	shaderManager = new ShaderManager();
	textureManager = new TextureManager();

	camera = new Camera(shaderManager, window, 60.0f, 1280.0f, 720.0f, .1f, 700.0f);
}

void Game::start() {
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//update camera
		camera->update();

		update();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwWindowShouldClose(window) == 0);

	//c.save_data(world.filename);

	glfwTerminate();
}