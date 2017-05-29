#include "BoundingSphere.h"

#include <iostream>


BoundingSphere::BoundingSphere(ObjMesh *pMesh, vec4 vec)
{
	float x = 0, y = 0, z = 0;
	float bigX, bigY, bigZ, smallX, smallY, smallZ;

	// find the largest and smallest x, y, z vertices to calculate the center of the mesh
	for (int i = 0; i < pMesh->m_iNumberOfVertices; i++) {
		if (i == 0) {
			bigX = smallX = pMesh->m_aVertexArray[i].x;
			bigY = smallY = pMesh->m_aVertexArray[i].y;
			bigZ = smallZ = pMesh->m_aVertexArray[i].z;
		}
		else {
			if (pMesh->m_aVertexArray[i].x > bigX) {
				bigX = pMesh->m_aVertexArray[i].x;
			}
			if (pMesh->m_aVertexArray[i].x < smallX) {
				smallX = pMesh->m_aVertexArray[i].x;
			}
			if (pMesh->m_aVertexArray[i].y > bigY) {
				bigY = pMesh->m_aVertexArray[i].y;
			}
			if (pMesh->m_aVertexArray[i].y < smallY) {
				smallY = pMesh->m_aVertexArray[i].y;
			}
			if (pMesh->m_aVertexArray[i].z > bigZ) {
				bigZ = pMesh->m_aVertexArray[i].z;
			}
			if (pMesh->m_aVertexArray[i].z < smallZ) {
				smallZ = pMesh->m_aVertexArray[i].z;
			}
		}
	}

	// now we should have the largest and smallest values of x, y, z
	// let's find the center of everything
	/*this->center.x = (bigX + smallX) / 2.0f;
	this->center.y = (bigY + smallY) / 2.0f;
	this->center.z = (bigZ + smallZ) / 2.0f;*/


	// let's find the largest distance from the middle, which will be our sphere's radius
	float edges[6] = { bigX, bigY, bigZ, smallX, smallY, smallZ };
	this->radius = 0;
	int largest = 0;
	for (int i = 0; i < 6; i++) {
		if (fabs(edges[i]) > largest) {
			this->radius = fabs(edges[i]);
			largest = i;
		}
	}
	std::cout << "\n The largest num was: " << edges[largest] << ", so our radius is: " << this->radius;


	/*this->x = x / pMesh->m_iNumberOfVertices;
	this->y = y / pMesh->m_iNumberOfVertices;
	this->z = z / pMesh->m_iNumberOfVertices;
*/

	this->center.x = vec.x;
	this->center.y = vec.y;
	this->center.z = vec.z;

	float d = sqrt(pow(bigX - this->center.x, 2) + pow(bigY - this->center.y, 2) + pow(bigZ - this->center.z, 2));

	

	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(this->center.x, this->center.y, this->center.z);
	glutWireSphere(this->radius, 30, 30);
	glPopMatrix();

	std::cout << "\n Num of verts: " << pMesh->m_iNumberOfVertices << "\n center x: " << this->center.x << " y: " << this->center.y << " z: " << this->center.z;
	std::cout << "\n Largest d: " << d << "\n";
}


BoundingSphere::~BoundingSphere()
{
}
