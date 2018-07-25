#include "TextureManager.h"

#include "../util.h"

TextureManager::TextureManager() {

}

void TextureManager::add(std::string name, GLuint texture) {
	textureMap[name] = texture;
}

void TextureManager::add(std::string name, const char *filename) {
	textureMap[name] = load_image(filename);
}

GLuint TextureManager::get(std::string name) {
	return textureMap[name];
}