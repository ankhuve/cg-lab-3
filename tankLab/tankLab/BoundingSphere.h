#pragma once

#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "objloader.h"

class BoundingSphere
{
public:
	BoundingSphere(ObjMesh *pMesh);
	~BoundingSphere();

	float x;
	float y;
	float z;
};

