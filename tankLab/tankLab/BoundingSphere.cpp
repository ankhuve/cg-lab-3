#include "BoundingSphere.h"

#include <iostream>


BoundingSphere::BoundingSphere(ObjMesh *pMesh)
{
	float x = 0, y = 0, z = 0;
	float bigX = 0, bigY = 0, bigZ = 0;

	for (int i = 0; i < pMesh->m_iNumberOfVertices; i++) {
		x += pMesh->m_aVertexArray[i].x;
		y += pMesh->m_aVertexArray[i].y;
		z += pMesh->m_aVertexArray[i].z;
	}

	this->x = x / pMesh->m_iNumberOfVertices;
	this->y = y / pMesh->m_iNumberOfVertices;
	this->z = z / pMesh->m_iNumberOfVertices;

	float bigD = 0;
	for (int i = 0; i < pMesh->m_iNumberOfVertices; i++) {
		float d = sqrt(pow(pMesh->m_aVertexArray[i].x - this->x, 2) + pow(pMesh->m_aVertexArray[i].y - this->y, 2) + pow(pMesh->m_aVertexArray[i].z - this->z, 2));
		if (d > bigD) { 
			bigD = d;
		}
	}

	/*glBegin(GL_LINE_LOOP);
		glLineWidth(10);
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(this->x, this->y, this->z);
		for (int i = 0; i <= 300; i++) {
			double angle = bigD * 3.14159265358979323846 * i / 300;
			double x = cos(angle);
			double y = sin(angle);
			glVertex2d(x, y);
			std::cout << x << " " << y << "\n";
		}
	glEnd();*/
	//glPointSize(10);
	//glPushMatrix();
	//glTranslatef(this->x, this->y, this->z);
	//glBegin(GL_POINTS);
	//
	//glColor3f(1.0, 1.0, 1.0);
	//for (int ii = 0; ii < 100; ii++)
	//{
	//	float theta = 2.0f * 3.1415926f * float(ii) / float(100);//get the current angle 

	//	float x = bigD * cosf(theta);//calculate the x component 
	//	float y = bigD * sinf(theta);//calculate the y component 

	//	glVertex3f(x + this->x, y + this->y, this->z);//output vertex 
	//	std::cout << x + this->x << " " << y + this->y << " " << this->z << "\n";

	//}
	//glEnd();
	//glPopMatrix();

	std::cout << "\n Num of verts: " << pMesh->m_iNumberOfVertices << "\n avg x: " << this->x << " y: " << this->y << " z: " << this->z;
	std::cout << "\n Largest d: " << bigD;
}


BoundingSphere::~BoundingSphere()
{
}
