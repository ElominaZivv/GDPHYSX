#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Object.h"
#include "Particle.h"
#include "../HeaderFiles/Camera.h"
#include "../HeaderFiles/Shader.h"

class Line {
	public:
		physics::Vector start, end; //anchor and ball pos
		physics::P6Particle* anchor;
		physics::P6Particle* sphere;
		GLuint VAO, VBO;

		mat4 transform_matrix;

		Line(physics::P6Particle* _anchor, physics::P6Particle* p);
		~Line();

		void update();
		void draw(Shader shader, Camera camera);
	private:
		void initialize();
		void updateVertexData();

};