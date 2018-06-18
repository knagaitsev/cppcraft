#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <array>
#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util.h"
#include "Camera.h"
#include "Controller.h"

#include "World\ChunkController.h"
#include "World\World.h"

GLFWwindow* window;

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

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
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
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
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

	Attrib attrib = { 0 };

	GLuint shaderProgram = load_shader("../../../shaders/vertex1.glsl", "../../../shaders/fragment1.glsl");

	attrib.program = shaderProgram;

	shaderProgram = load_shader("../../../shaders/vertex2.glsl", "../../../shaders/fragment1.glsl");

	attrib.program2 = shaderProgram;

	shaderProgram = load_shader("../../../shaders/vertex3.glsl", "../../../shaders/fragment1.glsl");

	attrib.program3 = shaderProgram;

	/*
	glEnableVertexAttribArray(attrib.position);
	glVertexAttribPointer(attrib.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(attrib.tex);
	glVertexAttribPointer(attrib.tex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	*/
	
	attrib.block_tex = load_image("../../../asset/gimp/blocks4.png");
	attrib.cat_tex = load_image("../../../asset/sample.png");
	attrib.crosshair = load_image("../../../asset/gimp/crosshair.png");
	attrib.lower_inventory = load_image("../../../asset/gimp/lower_inventory.png");
	attrib.lower_inventory_selector = load_image("../../../asset/gimp/lower_inventory_selector.png");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Camera camera(window, &attrib, 60.0f, 1280.0f, 720.0f, .1f, 700.0f);
	ChunkController c(&camera);
	Controller controller(window, &camera, &c);

	World world(&c, &camera, &controller);

	glClearColor(0.3f, 0.77f, 1.0f, 0.0f);

	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//update camera
		camera.update(&attrib);

		//drawing
		//glDrawElements(GL_TRIANGLES, NELEMS(elements), GL_UNSIGNED_INT, 0);
		c.draw(&attrib);

		world.update();

		//update controller
		controller.update(&attrib);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwWindowShouldClose(window) == 0);
	
	c.save_data(world.filename);

	glfwTerminate();

	return 0;
}
