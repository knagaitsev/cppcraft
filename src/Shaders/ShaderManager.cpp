#include "ShaderManager.h"

#include "../util.h"

ShaderManager::ShaderManager() {

}

void ShaderManager::add(std::string name, ShaderProgram program) {
	shaderMap[name] = program;
}

void ShaderManager::add(std::string name, const char *vertex_path, const char *fragment_path) {
	ShaderProgram s(vertex_path, fragment_path);
	shaderMap[name] = s;
}

ShaderProgram ShaderManager::get(std::string name) {
	return shaderMap[name];
}
