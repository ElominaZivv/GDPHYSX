#include "Vector.h"

namespace physics {
	/*
		Elomina, Zivv
		Assignment 02 - Vector Class

		Magnitude				/
		Direction				/
		Addition				/
		Subtraction				/
		Scalar Multiplication	/
		Component Product		/
		Scalar Product			/
		Vector Product			/
	*/

	Vector::Vector(): x(0), y(0), z(0) {};

	Vector::Vector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {};

	void Vector::debug()
	{
		std::cout << "		x: " << x << "		y: " << y << "		z: " << z << std::endl;
	}

	// turns vector class into glm::vec3
	// "explicit couldn't be specified for out-of-class member function definition; it should be specified only inside the class definition."
	Vector::operator glm::vec3() const { return glm::vec3(x, y, z); }

	Vector Vector::normalize()
	{
		Vector normalized = this->dir();
		this->x = normalized.x;
		this->y = normalized.y;
		this->z = normalized.z;
		return normalized;
	}

	// Computes for the Magnitude
	float Vector::mag()
	{
		return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	}

	// Computes for the Squared Magnitude
	float Vector::squareMag()
	{
		return (pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	//Computes for Direction
	physics::Vector Vector::dir()
	{
		float magnitude = mag();
		if (magnitude == 0)
		{
			//cout << "You cannot divide components of vector by magnitude = 0. Returning physics::Vector(0,0,0)" << endl;
			return physics::Vector(0, 0, 0);
		}
		return physics::Vector(x/magnitude,y/magnitude,z/magnitude);
	}

	// Addition operation
	Vector Vector::operator+ (const Vector rhs) 
	{
		// this == left hand side of the addition operation
		// parameter/rhs == right hand side of the operation
		return Vector(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
	}

	void Vector::operator+=(const Vector rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
	}

	//Subtraction operation
	Vector Vector::operator- (const Vector rhs) 
	{
		// this == left hand side of the subtraction operation
		// parameter/rhs == right hand side of the operation
		return Vector(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	void Vector::operator-=(const Vector rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
	}

	// Scalar multiplication
	Vector Vector::operator* (const float s) 
	{
		return Vector(this->x * s, this->y * s, this->z * s);
	}

	// Component Product
	Vector Vector::compProd (const Vector rhs)
	{
		/*
				Ax*Bx = Cx
				Ay*By = Cy
				Az*Bz = Cz
		*/
		return Vector(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
	}

	//Scalar Product / Dot Product
	float Vector::dot (const Vector rhs)
	{
		// this == left hand side of scalar product/dot product operation
		// parameter/rhs == right hand side of the operation
		// A-> * B-> = AxBx + AyBy + AzBz
		return (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);
	}

	//Vector Product / Cross Product
	Vector Vector::cross (const Vector rhs)
	{
		// this == left hand side of vector product / cross product operation
		// parameter/rhs == right hand side of the operation
		// A-> x B-> = i->(AyBz - AzBy) - j->(AxBz - AzBx) + z->(AxBy - AyBx)
		return Vector((this->y * rhs.z) - (this->z * rhs.y), -((this->x * rhs.z) - (this->z * rhs.x)), (this->x * rhs.y) - (this->y * rhs.x));
	}

	//Equal to
	bool Vector::operator== (const Vector rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	//Greater than equal to
	bool Vector::operator>= (const Vector rhs)
	{
		return (x >= rhs.x && y >= rhs.y && z >= rhs.z);
	}

	//Greater than
	bool Vector::operator> (const Vector rhs)
	{
		return (x > rhs.x && y > rhs.y && z > rhs.z);
	}

	//Lesser than equal to
	bool Vector::operator<= (const Vector rhs)
	{
		return (x <= rhs.x && y <= rhs.y && z <= rhs.z);
	}

	//Lesser than
	bool Vector::operator< (const Vector rhs)
	{
		return (x < rhs.x && y < rhs.y && z < rhs.z);
	}
}