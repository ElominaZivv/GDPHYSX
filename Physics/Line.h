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
		physics::P6Particle* sphere;
		float lineX1, lineX2, lineY1, lineY2;
		GLuint VAO, VBO;

		Line(Object* anchor, physics::P6Particle* p);
		~Line();

		void update();
		void draw();
	private:
		void initialize();
		void updateVertexData();

};