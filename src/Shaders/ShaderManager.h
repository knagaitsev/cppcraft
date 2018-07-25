#ifndef _ShaderManager_h_
#define _ShaderManager_h_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <map>
#include <vector>
#include <algorithm>

#include "ShaderProgram.h"


class ShaderManager {
private:
	std::map<std::string, ShaderProgram> shaderMap;
protected:
public:
	ShaderManager();
	void add(std::string name, ShaderProgram program);
	void add(std::string name, const char *vertex_path, const char *fragment_path);
	ShaderProgram get(std::string name);
};

#endif