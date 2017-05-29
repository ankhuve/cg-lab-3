#pragma once

#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "objloader.h"
#include <vector>

struct vec3 {
	float x, y, z;
};

struct vec4 {
	float x, y, z, s;
};

struct pos {
	float x, y, z;
};

class BoundingSphere
{
public:
	BoundingSphere();
	BoundingSphere(ObjMesh * pMesh, pos currGlobalPosition);
	BoundingSphere(std::vector<BoundingSphere> &boundingSpheres);
	bool inBounds(pos position);
	void drawBoundingSphere();
	//~BoundingSphere();

	vec3 center;

	float radius;

	bool isHitByProjectile;

	float x;
	float y;
	float z;

	vec3 globalPos;
};

