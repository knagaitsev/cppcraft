#ifndef _util_h_
#define _util_h_

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <GL/glew.h>

#include <glm/glm.hpp>

#define PI 3.14159265359
#define DEGREES(radians) ((radians) * 180 / PI)
#define RADIANS(degrees) ((degrees) * PI / 180)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIGN(x) (((x) > 0) - ((x) < 0))
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int rand_int(int n);

double rand_double();

std::string read_file(const char *filePath);

GLuint load_shader(const char *vertex_path, const char *fragment_path);

void load_png_texture(const char *file_name);

GLuint load_image(const char *filename);

glm::vec3 rotate_vec3(glm::vec3 vec, float angle, glm::vec3 axis);

void print_vec3(glm::vec3 vec);

typedef struct {
	GLuint program;
	GLuint program2;
	GLuint program3;
	GLuint block_tex;
	GLuint cat_tex;
	GLuint crosshair;
	GLuint lower_inventory;
	GLuint lower_inventory_selector;
} Attrib;

#endif