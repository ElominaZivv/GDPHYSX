#ifndef VECTOR_H
#define VECTOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

namespace physics {
	class Vector { //	My cursed vector operations
	public:
		float x, y, z;

		Vector();
		Vector(const float _x, const float _y, const float _z);

		//Debug
		void debug();

		// Converts Vector -> glm::vec3
		// "explicit couldn't be specified for out-of-class member function definition; it should be specified only inside the class definition."
		explicit operator glm::vec3() const;

		//Magnitude
		float mag();

		//Direction
		physics::Vector dir();

		// Addition
		Vector operator+ (const Vector rhs);

		// Addition +=
		void operator+= (const Vector rhs);

		//Subtraction
		Vector operator- (const Vector rhs);

		//Subtraction -=
		void operator-= (const Vector rhs);

		//Scalar Multiplication
		Vector operator* (const float s);

		//Component Product
		Vector compProd(const Vector rhs);

		//Scalar Product / Dot Product
		float dot(const Vector rhs);

		//Vector Product / Cross Product
		Vector cross(const Vector rhs);

		//Comparisons
		//Equal to
		bool operator== (const Vector rhs);
		
		//Greater than equal to
		bool operator>= (const Vector rhs);
		
		//Greater than
		bool operator> (const Vector rhs);

		//Lesser than equal to
		bool operator<= (const Vector rhs);

		//Lesser than
		bool operator< (const Vector rhs);


	};
}

#endif