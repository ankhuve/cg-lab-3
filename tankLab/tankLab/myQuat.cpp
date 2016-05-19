#include "MyQuat.h"
#include <iostream>

using namespace MyMathLibrary;


MyQuat::MyQuat(void)
{
	this->w = 0;
	this->v = MyVector(0, 0, 0);
}

MyQuat::MyQuat(float angleDeg, MyVector &axis)
{
	this->w = cos(angleDeg / 2);

	axis.normalise();
	float s = sin(angleDeg / 2);

	this->v.x = axis.x * s;
	this->v.y = axis.y * s;
	this->v.z = axis.z * s;
}

MyQuat::MyQuat(Position &p)
{
	this->w = 0;
	this->v = MyVector(p.x, p.y, p.z);
}

MyQuat MyQuat::addTo(const MyQuat &other) const
{
	MyQuat result;
	result.w = this->w + other.w;
	result.v = this->v.addTo(other.v);
	std::cout << result.w << "\n";
	return result;
}

MyQuat MyQuat::multiplyBy(const MyQuat &other) const
{
	MyQuat result;
	result.w = (this->w * other.w) - (this->v.getDotProduct(other.v));
	MyVector v1 = this->v.getCrossProduct(other.v);
	MyVector v2 = MyVector(this->w * other.v.x, this->w * other.v.y, this->w * other.v.z);
	MyVector v3 = MyVector(other.w * this->v.x, other.w * this->v.y, other.w * this->v.z);
	MyVector vTot;
	vTot = v1.addTo(v2).addTo(v3);

	result.v = vTot;

	return result;
}

float MyQuat::getMagnitude(void) const
{
	float magnitude = sqrt(pow(this->w, 2) + pow(this->v.x, 2) + pow(this->v.y, 2) + pow(this->v.z, 2));
	return magnitude;
}

void MyQuat::normalise(void)
{
	float magnitude = this->getMagnitude();
	this->w = this->w / magnitude;
	this->v.x = this->v.x / magnitude;
	this->v.y = this->v.y / magnitude;
	this->v.z = this->v.z / magnitude;
}

MyQuat MyQuat::getConjugate(void) const
{
	MyQuat result;
	result.w = this->w;
	result.v = MyVector(-this->v.x, -this->v.y, -this->v.z);

	return result;
}

//MyQuat MyQuat::getInverse(void) const
//{
//	
//}

MyMatrix MyQuat::convertToRotationMatrix(void) const
{
	MyMatrix result;
	GLfloat rotMatrix[16];

	float xx = this->w * this->w;
	float xy = this->w * this->v.x;
	float xz = this->w * this->v.y;
	float xw = this->w * this->v.z;
	float yy = this->v.x * this->v.x;
	float yz = this->v.x * this->v.y;
	float yw = this->v.x * this->v.z;
	float zz = this->v.y * this->v.y;
	float zw = this->v.y * this->v.z;

	rotMatrix[0] = 1 - 2 * (yy + zz);
	rotMatrix[1] = 2 * (xy - zw);
	rotMatrix[2] = 2 * (xz + yw);
	rotMatrix[4] = 2 * (xy + zw);
	rotMatrix[5] = 1 - 2 * (xx + zz);
	rotMatrix[6] = 2 * (yz - xw);
	rotMatrix[8] = 2 * (xz - yw);
	rotMatrix[9] = 2 * (yz + xw);
	rotMatrix[10] = 1 - 2 * (xx + yy);
	rotMatrix[3] = rotMatrix[7] = rotMatrix[11] = rotMatrix[12] = rotMatrix[13] = rotMatrix[14] = 0;
	rotMatrix[15] = 1;

	result.setMyMatrix(rotMatrix);
	return result;
}