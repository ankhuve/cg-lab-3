#include "myMatrix.h"
#include <iostream>

//using namespace MyMathLibrary;

MyMatrix::MyMatrix(void)
{
	this->loadIdentity();
}

void MyMatrix::loadIdentity(void)
{
	static GLfloat identityMatrix[16] =
	{
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0
	};
	memcpy(this->myMatrix, identityMatrix, sizeof(identityMatrix));
}

void MyMatrix::getGLMatrix(GLenum pname) 
// Returns any matrix (used by other functions: do use directly)
{
	//remember, you should have a GLfloat myMatrix[16]; defined
	//as a member of your class (see above)
	glGetFloatv(pname,this->myMatrix);
}

void MyMatrix::getGLModelviewMatrix(void) 
// Returns the current modelview matrix
{
	getGLMatrix(GL_MODELVIEW_MATRIX);
}

void MyMatrix::getGLProjectionMatrix(void) 
// Returns the current projection matrix
{
	getGLMatrix(GL_PROJECTION_MATRIX);
}

void MyMatrix::multiplyGLMatrix(void) 
// Multiply our matrix by the current OpenGL one
//Remember to first set the current GL matrix by using the command:
// glMatrixMode()!!!
{
	glMultMatrixf(this->myMatrix);
}

void MyMatrix::setGLMatrix(void)
// Set the OpenGL matrix to that currently specified in myMatrix
//Remember to first set the current GL matrix by using the command:
// glMatrixMode()!!!
{
	glLoadMatrixf(this->myMatrix);
}

void MyMatrix::printMatrix(void)
{
	int numberOfValues = sizeof(this->myMatrix) / sizeof(this->myMatrix[0]);
	for (int i = 0; i < numberOfValues; i++) {
		std::cout << this->myMatrix[i] << "  ";
		if ((i + 1) % 4 == 0) {
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}

void MyMatrix::translate(float x, float y, float z)
{
	this->myMatrix[12] = x;
	this->myMatrix[13] = y;
	this->myMatrix[14] = z;
}

void MyMatrix::rotateZ(float rotation)
{
	float rads = (rotation * 3.141592653589793 / 180.0); // convert degrees to radians
	this->myMatrix[0] = cos(rads);
	this->myMatrix[1] = sin(rads);
	this->myMatrix[4] = -sin(rads);
	this->myMatrix[5] = cos(rads);
}

void MyMatrix::setMyMatrix(GLfloat mat[16])
{
	memcpy(this->myMatrix, mat, sizeof(mat));
}