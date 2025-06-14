#ifndef MODEL3D_H
#define MODEL3D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.h"
#include "Shader.h"
#include "Camera.h"
#include "../Physics/Vector.h"

#include <iostream>
#include <string>

using namespace std;

class Model3D{
private:
	shared_ptr<VAO> modelVAO;
	mat4 identity_matrix, transform_matrix;

public:
	vec3 modelPos;
	vec3 color, size;
	Model3D(shared_ptr<VAO> newVAO);


	void draw(Shader shader, Camera camera);
	void setModelPos(float x, float y, float z);
	void setModelPosVec(physics::Vector vecPos);
	void setSize(float _size);
};

#endif