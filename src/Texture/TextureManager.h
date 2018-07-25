#ifndef _TextureManager_h_
#define _TextureManager_h_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <map>
#include <vector>
#include <algorithm>

#include <GL/glew.h>

class TextureManager {
private:
	std::map<std::string, GLuint> textureMap;
protected:
public:
	TextureManager();
	void add(std::string name, GLuint texture);
	void add(std::string name, const char *filename);
	GLuint get(std::string name);
};

#endif