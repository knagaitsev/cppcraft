#include "ShaderProgram.h"

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iterator>

#include "../util.h"

ShaderProgram::ShaderProgram() {

}

ShaderProgram::ShaderProgram(const char *vertex_path, const char *fragment_path) {
	program = load_shader(vertex_path, fragment_path);
}