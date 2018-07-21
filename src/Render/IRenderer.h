#ifndef _IRenderer_h_
#define _IRenderer_h_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../util.h"

class IRenderer {
private:
	GLuint vbo;
	GLuint ebo;
	int elements_size;
	int vertices_size;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> elements;

	struct Triangle {
		int index;
		float distance;
	};

	void bind_texture(GLuint texture);

	void bind_buffer();
	void unbind_buffer();

	void bind_program();
	void unbind_program();
protected:
	const int vertex_size = 5;

	void gen_vertices_buffer(std::vector<GLfloat> *vertices);
	void gen_elements_buffer(std::vector<GLuint> *elements);
public:
	void draw(GLuint program, GLuint texture);
	void draw_transparent(GLuint program, GLuint texture, glm::vec3 player_pos);
	void delete_buffers();

	//virtual GLuint get_program(Attrib *attrib);
	//virtual GLuint get_texture(Attrib *attrib);
};

#endif