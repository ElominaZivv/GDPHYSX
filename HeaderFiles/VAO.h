#ifndef VAO_H
#define VAO_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "tiny_obj_loader.h"

using namespace std;

class VAO {
private:
	string filePath, warning, error;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> material;
	tinyobj::attrib_t attributes;

	vector<GLuint> meshIndices;
	vector<GLfloat> fullVertexData;

	GLuint vao, vbo;

public:
	VAO(std::string objFilePath);
	~VAO();
	GLuint getVAO();
	vector<GLfloat> getFullVertexData();
};

#endif