#ifndef SHADERPROG_H
#define SHADERPROG_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tiny_obj_loader.h"

using namespace std;
using namespace glm;


using namespace std;
using namespace glm;

class Shader
{
private:
	GLuint shaderProg, vertexShader, fragmentShader;
public:
	Shader(string vertFilePath, string fragFilePath);
	void activate();
	GLuint getShader();
	~Shader();
};

#endif