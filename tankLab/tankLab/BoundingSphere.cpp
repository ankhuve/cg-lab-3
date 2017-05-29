#include "BoundingSphere.h"
#include "myVector.h"

#include <iostream>
#include <vector>


BoundingSphere::BoundingSphere(ObjMesh *pMesh, pos currGlobalPosition)
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
	this->center.x = (bigX + smallX) / 2.0f;
	this->center.y = (bigY + smallY) / 2.0f;
	this->center.z = (bigZ + smallZ) / 2.0f;

	// let's find the largest distance from the middle, which will be our sphere's radius
	//float edges[6] = { bigX, bigY, bigZ, smallX, smallY, smallZ };
	//this->radius = 0;
	//int largest = 0;
	//for (int i = 0; i < 6; i++) {
	//	if (fabs(edges[i]) > largest) {
	//		this->radius = fabs(edges[i]);
	//		largest = i;
	//	}
	//}
	//std::cout << "\n The largest num was: " << edges[largest] << ", so our radius is: " << this->radius;

	
	// calculate the radius of the bounding sphere
	float d = sqrt(pow(bigX - this->center.x, 2) + pow(bigY - this->center.y, 2) + pow(bigZ - this->center.z, 2));
	this->radius = d;
	//std::cout << "\n The largest distance was: " << d << ", so our radius is: " << this->radius;

	// draw the sphere
	glPushMatrix();
		// translate to the center of the mesh
		glTranslatef(this->center.x, this->center.y, this->center.z);

		// save the global position of the bounding sphere
		this->globalPos.x = currGlobalPosition.x + this->center.x;
		this->globalPos.y = currGlobalPosition.y + this->center.y;
		this->globalPos.z = currGlobalPosition.z + this->center.z;

		// draw the bounding sphere with d as the radius
		//if (isHitByProjectile) {
			//glutWireSphere(this->radius, 15, 15);
		//}

	glPopMatrix();

	//std::cout << "\n Num of verts: " << pMesh->m_iNumberOfVertices << "\n BoundingSphere global pos x: " << this->globalPos.x << " y: " << this->globalPos.y << " z: " << this->globalPos.z;
	//std::cout << "\n Largest d: " << this->radius << "\n";
}

BoundingSphere::BoundingSphere(std::vector<BoundingSphere> &boundingSpheres) {
	int numSpheres = boundingSpheres.size();
	float sumX = 0, sumY = 0, sumZ = 0;
	float avgX, avgY, avgZ;
	float bigR = 0;
	for (int i = 0; i < (numSpheres - 1); i++)
	{
		BoundingSphere b = boundingSpheres[i];
		//std::cout << "\n bs: " << b.globalPos.x << " " << b.globalPos.y << " " << b.globalPos.z;

		sumX += b.globalPos.x;
		sumY += b.globalPos.y;
		sumZ += b.globalPos.z;


		// find the largest radius
		if (b.radius > bigR) {
			bigR = b.radius;
		}
	}

	// for now, we'll just calculate the average values of each axis to get a somewhat correct placement for the sphere
	avgX = sumX / numSpheres;
	avgY = sumY / numSpheres;
	avgZ = sumZ / numSpheres;

	this->center.x = avgX;
	this->center.y = avgY;
	this->center.z = avgZ;

	//printf("\nBigSphere center: %f : %f : %f", avgX, avgY, avgZ);

	// go through each of the bounding spheres to find the largest (radius + global pos) combination
	float newRadius = 0.0f;
	for (int i = 0; i < (numSpheres - 1); i++)
	{
		BoundingSphere b = boundingSpheres[i];
		// distance from this sphere's center to the small sphere's center
		float d = sqrt(pow(avgX - b.globalPos.x, 2) + pow(avgY - b.globalPos.y, 2) + pow(avgZ - b.globalPos.z, 2));

		// add the small sphere's radius to reach all the way out
		d = d + b.radius;
		if (d > newRadius) {
			newRadius = d;
		}
	}

	this->radius = newRadius;

	//// draw the sphere
	//glPushMatrix();
	//	// translate to the center of the mesh
	//	glTranslatef(avgX, avgY, avgZ);

		// save the global position of the bounding sphere
		this->globalPos.x = avgX;
		this->globalPos.y = avgY;
		this->globalPos.z = avgZ;

	//	// draw the bounding sphere with d as the radius
	//	/*if (isHitByProjectile) {
	//		glutWireSphere(this->radius, 30, 30);
	//	}*/
	//glPopMatrix();
}

BoundingSphere::BoundingSphere()
{

}

void BoundingSphere::drawBoundingSphere()
{
	//
	//// draw the bounding sphere with d as the radius
	////if (this->isHitByProjectile == true) {
	//	glPushMatrix();
	//		//glScalef(0.1, 0.1, 0.1);
	//		glutWireSphere(this->radius, 30, 30);
	//	glPopMatrix();
	////}


	// draw the sphere if drawn directly after it has been created
	//glPushMatrix();
		// draw the bounding sphere with d as the radius
		if (isHitByProjectile) {
			//printf("\nPart was hit! \nCenter: %f, %f, %f \nGlobal: %f, %f, %f\n", this->center.x, this->center.y, this->center.z, this->globalPos.x, this->globalPos.y, this->globalPos.z);
			// draw the sphere
			glPushMatrix();
			// translate to the center of the mesh
			glTranslatef(this->center.x, this->center.y, this->center.z);

			// draw the bounding sphere with d as the radius
			glutWireSphere(this->radius, 30, 30);

			glPopMatrix();
		}
	//glPopMatrix();
}


// check if the incoming position is inside of the bounding sphere
bool BoundingSphere::inBounds(pos shotPos)
{
	float d = sqrt(pow(shotPos.x - this->globalPos.x, 2) + pow(shotPos.y - this->globalPos.y, 2) + pow(shotPos.z - this->globalPos.z, 2));
	//printf("\nDifference: %f (radius: %f, d: %f)", d - this->radius, this->radius, d);
	if (d < this->radius)
	{
		this->isHitByProjectile = true;
		return true;
	}
	else {
		this->isHitByProjectile = false;
		return false;
	}
}