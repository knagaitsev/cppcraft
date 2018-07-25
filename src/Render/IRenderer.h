#ifndef _IRenderer_h_
#define _IRenderer_h_

#include <vector>

#include <string.h>

#include <GL/glew.h>

#include "../util.h"

#include "../Game.h"

#include "../Shaders/ShaderProgram.h"

class IRenderer : public Game {
private:
	GLuint vbo;
	GLuint ebo;

	void bind_texture(std::string textureKey);

	void bind_buffer();
	void unbind_buffer();

	void bind_program(std::string key);
	void unbind_program(std::string key);
protected:
	int elements_size;
	int vertices_size;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> elements;

	struct Triangle {
		int index;
		float distance;
	};

	const int vertex_size = 5;

	void gen_vertices_buffer(std::vector<GLfloat> *vertices);
	void gen_elements_buffer(std::vector<GLuint> *elements);

	void draw_triangles(int start, int length);

	void begin(std::string programKey, std::string textureKey);
	void end(std::string programKey);
public:
	void update();
	void draw(std::string programKey, std::string textureKey);
	void delete_buffers();

	//virtual GLuint get_program(Attrib *attrib);
	//virtual GLuint get_texture(Attrib *attrib);
};

#endif