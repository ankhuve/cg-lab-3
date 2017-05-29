#pragma once

#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "objloader.h"

struct vec3 {
	float x, y, z;
};

struct vec4 {
	float x, y, z, s;
};

class BoundingSphere
{
public:
	BoundingSphere(ObjMesh * pMesh, vec4 vec);
	~BoundingSphere();

	vec3 center;

	float radius;

	float x;
	float y;
	float z;
};

