#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Object.h"
#include "Particle.h"

class Line {
	public:
		float x1, y1, x2, y2; //anchor and ball pos
		float lineX1, lineX2, lineY1, lineY2;
		GLuint VAO, VBO;

		Line(Object* anchor, physics::P6Particle* p);
		void update(physics::P6Particle* p);
		void draw();
	private:
		void initialize();
		void updateVertexData();

};