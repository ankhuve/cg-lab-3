#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"
#include <iostream>

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include <mmsystem.h>

#ifndef MYVECTOR_AND_MYMATRIX_H_INCLUDED
#define MYVECTOR_AND_MYMATRIX_H_INCLUDED

#include "myvector.h"
#include "mymatrix.h"

//using namespace MyMathLibrary;

#include "stdlib.h"
#include "stdio.h"

#include "objloader.h"

#include "BoundingSphere.h"
#include <vector>

//#ifndef CPARTICLESYSTEM_H_INCLUDED
//#define CPARTICLESYSTEM_H_INCLUDED
#include "particleSystem.h"


ObjMesh* tankBody;
ObjMesh* tankTurret;
ObjMesh* tankMainGun;
ObjMesh* tankSecondaryGun;
ObjMesh* tankWheel;

ObjMesh* tankBodyLow;
ObjMesh* tankTurretLow;
ObjMesh* tankMainGunLow;
ObjMesh* tankSecondaryGunLow;
ObjMesh* tankWheelLow;


void load_tank_objs(void);
void load_low_detail_tank_objs(void);

float zPos = -30.0;
float yRot = 0.0;

float turretRot = 0.0;
float secondaryGunRot = 0.0;
float wheelRot = 0.0;
bool flipWheelRot = false;
pos currGlobalPos;
pos interactiveShotPos;
std::vector<int> hitList(4);
int interactiveShotType = 1;
bool tankPartWasHit = false;

vec4 tankPartPositions[4];
std::vector<BoundingSphere> boundingSpheres;

GLuint tankBodyList, tankTurretList, tankMainGunList, tankSecondaryGunList, tankWheelList;
GLuint tankBodyLowList, tankTurretLowList, tankMainGunLowList, tankSecondaryGunLowList, tankWheelLowList;

//define the particle systems
int g_nActiveSystem = 0;
CParticleSystem *g_pParticleSystems[7];
float  g_fElpasedTime;
double g_dCurTime;
double g_dLastTime;
void initParticles(void);

//prototypes for our callback functions
void draw(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation
void createWheel(float x, float y, float z);
void drawMesh(ObjMesh *pMesh, bool useTextures);
void addToTankPosList(vec4 &vec, float x, float y, float z, float s);
void fireAway(void);
//void drawHitBoundingSpheres(float x, float y, float z);
void drawInteractiveShot(float x, float y, float z);
void drawBoundingSphereIfHit(BoundingSphere bs, int i);
void draw_tank_low(float x, float y, float z);

//our main routine
int main(int argc, char *argv[])
{
  //Initialise Glut and create a window
  glutInit(&argc, argv);
  //sets up our display mode
  //here we've selected an RGBA display with depth testing 
  //and double buffering enabled
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  //create a window and pass through the windows title
  glutCreateWindow("Basic Glut Application");

  currGlobalPos.x = 0.0;
  currGlobalPos.y = 0.0;
  currGlobalPos.z = 0.0;

  interactiveShotPos.x = -10.0f;
  interactiveShotPos.y = 40.0f;
  interactiveShotPos.z = 25.0f;

  hitList[0] = -1;
  hitList[1] = -1;
  hitList[2] = -1;
  hitList[3] = -1;


  //run our own drawing initialisation routine
  init_drawing();

  load_tank_objs();
  load_low_detail_tank_objs();

  initParticles();

  //tell glut the names of our callback functions point to our 
  //functions that we defined at the start of this file
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutDisplayFunc(draw);

  //request a window size of 600 x 600
  glutInitWindowSize(600,600);
  glutReshapeWindow(600,600);

  //go into the main loop
  //this loop won't terminate until the user exits the 
  //application
  glutMainLoop();
  return 0;
}

void load_low_detail_tank_objs(void)
{
	tankBodyLow = LoadOBJ(".\\tankobjs\\tankbodyLow.obj");
	tankTurretLow = LoadOBJ(".\\tankobjs\\tankturretLow.obj");
	tankMainGunLow = LoadOBJ(".\\tankobjs\\tankmaingunLow.obj");
	tankSecondaryGunLow = LoadOBJ(".\\tankobjs\\tanksecondarygunLow.obj");
	tankWheelLow = LoadOBJ(".\\tankobjs\\tankwheelLow.obj");

	tankBodyLowList = glGenLists(5);
	tankTurretLowList = tankBodyLowList + 1;
	tankMainGunLowList = tankTurretLowList + 1;
	tankSecondaryGunLowList = tankMainGunLowList + 1;
	tankWheelLowList = tankSecondaryGunLowList + 1;


	glNewList(tankBodyLowList, GL_COMPILE);
	//DrawOBJ(tankBody->m_iMeshID);
	drawMesh(tankBodyLow, false);
	glEndList();

	glNewList(tankTurretLowList, GL_COMPILE);
	//DrawOBJ(tankTurret->m_iMeshID);
	drawMesh(tankTurretLow, false);
	glEndList();

	glNewList(tankMainGunLowList, GL_COMPILE);
	//DrawOBJ(tankMainGun->m_iMeshID);
	drawMesh(tankMainGunLow, false);
	glEndList();

	glNewList(tankSecondaryGunLowList, GL_COMPILE);
	//DrawOBJ(tankSecondaryGun->m_iMeshID);
	drawMesh(tankSecondaryGunLow, false);
	glEndList();

	glNewList(tankWheelLowList, GL_COMPILE);
	//DrawOBJ(tankWheel->m_iMeshID);
	drawMesh(tankWheelLow, false);
	glEndList();
}


void load_tank_objs(void)
{
  tankBody = LoadOBJ(".\\tankobjs\\tankbody.obj");
  tankTurret = LoadOBJ(".\\tankobjs\\tankturret.obj");
  tankMainGun = LoadOBJ(".\\tankobjs\\tankmaingun.obj");
  tankSecondaryGun = LoadOBJ(".\\tankobjs\\tanksecondarygun.obj");
  tankWheel = LoadOBJ(".\\tankobjs\\tankwheel.obj");
  SetTextures(tankBody->m_iMeshID, NULL, ".\\tankobjs\\texture.tga");

  // the number of bounding spheres we will have (all parts + one for the whole model)
  boundingSpheres.resize(5);
  
  tankBodyList = glGenLists(5);
  tankTurretList = tankBodyList + 1;
  tankMainGunList = tankTurretList + 1;
  tankSecondaryGunList = tankMainGunList + 1;
  tankWheelList = tankSecondaryGunList + 1;

  glNewList(tankBodyList, GL_COMPILE);
	//DrawOBJ(tankBody->m_iMeshID);
	drawMesh(tankBody, true);
  glEndList();

  glNewList(tankTurretList, GL_COMPILE);
	//DrawOBJ(tankTurret->m_iMeshID);
	drawMesh(tankTurret, true);
  glEndList();

  glNewList(tankMainGunList, GL_COMPILE);
	//DrawOBJ(tankMainGun->m_iMeshID);
	drawMesh(tankMainGun, true);
  glEndList();

  glNewList(tankSecondaryGunList, GL_COMPILE);
	//DrawOBJ(tankSecondaryGun->m_iMeshID);
	drawMesh(tankSecondaryGun, true);
  glEndList();

  glNewList(tankWheelList, GL_COMPILE);
	//DrawOBJ(tankWheel->m_iMeshID);
	drawMesh(tankWheel, true);
  glEndList();

  
}

void translateAndSavePos(float x, float y, float z) {
	glTranslatef(x, y, z);
	currGlobalPos.x += x;
	currGlobalPos.y += y;
	currGlobalPos.z += z;
}

void draw_tank(float x, float y, float z)
{
	glRotatef(-90, 0.0, 1.0, 0.0);
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);

		//glColor3f(1.0f, 0.0f, 0.0f);
		translateAndSavePos(x,y,z);

		glScalef(0.1,0.1,0.1);		//reduce the size of the tank on screen

		glEnable(GL_TEXTURE_2D); // d i s a b l e l i g h t i n g
		glEnable(GL_LIGHTING); // d i s a b l e t e x t u ri n g

		glCallList(tankBodyList);
		addToTankPosList(tankPartPositions[0], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

		//std::cout << "\nTank body:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;
		
		//glColor4f(1.0f, 0.0, 0.0, 0.2f);
		BoundingSphere tankBodyBound = BoundingSphere(tankBody, currGlobalPos);
		drawBoundingSphereIfHit(tankBodyBound, 0);
		//glColor4f(1.0f, 0.0, 0.0, 1.0f);
	
		//Use your own draw code here to draw the rest of the tank
		//Here's the code for each individual part
		//Each part is placed with respect to the origin
		//you'll need to add in glPushMatrix/glTranslatef/glRotatef/glPopMatrix commands as necessary
		glPushMatrix();
			//glColor3f(0.0f, 1.0f, 0.0f);
			glRotatef(turretRot, 0.0, 1.0, 0.0);
			translateAndSavePos(0.0f, 15.0f, 0.0f);
			glCallList(tankTurretList);
			addToTankPosList(tankPartPositions[1], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);
			
			//std::cout << "\nTank turret:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;
			
			//glColor4f(0.0f, 1.0, 0.0, 0.2f);
			BoundingSphere tankTurretBound = BoundingSphere(tankTurret, currGlobalPos);
			drawBoundingSphereIfHit(tankTurretBound, 1);
			//std::cout << "\nBoundingSphere center: " << tankTurretBound.center.x << ", " << tankTurretBound.center.y << ", " << tankTurretBound.center.z;
			
		

			glPushMatrix();
				//glColor4f(0.0f, 1.0, 0.0, 1.0f);
				//glColor3f(1.0f, 1.0f, 0.0f);
				translateAndSavePos(54.0f, -102.0f, 10.0f);
				glCallList(tankMainGunList);
				addToTankPosList(tankPartPositions[2], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

				//std::cout << "\nTank main gun:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;
				
				//glColor4f(1.0, 1.0, 0.0, 0.2);
				BoundingSphere tankMainGunBound = BoundingSphere(tankMainGun, currGlobalPos);
				drawBoundingSphereIfHit(tankMainGunBound, 2);

				translateAndSavePos(-54.0f, 102.0f, -10.0f);

			glPopMatrix();

			glPushMatrix();
				//glColor3f(0.0f, 0.0f, 1.0f);
				translateAndSavePos(-12.0f, 17.0f, -15.0f);
				glRotatef(secondaryGunRot, 0.0, 1.0, 0.0);
				translateAndSavePos(0.0f, 0.0f, 10.0f);
				glCallList(tankSecondaryGunList);
				addToTankPosList(tankPartPositions[3], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

				//std::cout << "\nTank secondary gun:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;
				
				BoundingSphere tankSecondaryGunBound = BoundingSphere(tankSecondaryGun, currGlobalPos);
				drawBoundingSphereIfHit(tankSecondaryGunBound, 3);

				/*GLfloat matrix[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
				int i = 0;
				for each (float f in matrix)
				{
					if (i % 3 == 0) {
						printf("\n%f", f);
					}
					else {
						printf(" %f", f);
					}
					i++;
				}*/

			glPopMatrix();

		glPopMatrix();

		glPushMatrix();
			//glColor4f(0.0, 0.0, 1.0, 1.0f);
			//glColor3f(1.0f, 1.0f, 1.0f);
			flipWheelRot = false;
			int zWheelPos = 43;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 20;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);

			flipWheelRot = true;
			glRotatef(180, 0.0, 1.0, 0.0);

			zWheelPos = -43;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 20;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
		glPopMatrix();

/*		currGlobalPos.x = 0.0;
		currGlobalPos.y = 0.0;
		currGlobalPos.z = 0.0;*/
		//glColor3f(1.0f, 1.0f, 1.0f);
		//std::cout << "\nFull tank:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;

		
		boundingSpheres[0] = tankBodyBound;
		boundingSpheres[1] = tankTurretBound;
		boundingSpheres[2] = tankMainGunBound;
		boundingSpheres[3] = tankSecondaryGunBound;
		BoundingSphere fullTankBound = BoundingSphere(boundingSpheres);
		boundingSpheres[4] = fullTankBound;
		

	glPopMatrix();

}

// draw low detail version of tank
void draw_tank_low(float x, float y, float z)
{
	glRotatef(-90, 0.0, 1.0, 0.0);
	glPushMatrix();
		//glColor3f(1.0f, 0.0f, 0.0f);
		translateAndSavePos(x, y, z);
		glScalef(0.1, 0.1, 0.1);		//reduce the size of the tank on screen

		// draw body
		glDisable(GL_TEXTURE_2D); // d i s a b l e l i g h t i n g
		glDisable(GL_LIGHTING); // d i s a b l e t e x t u ri n g
		glColor3ub(83, 68, 49);

		glCallList(tankBodyLowList);
		addToTankPosList(tankPartPositions[0], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

		BoundingSphere tankBodyBound = BoundingSphere(tankBody, currGlobalPos);
		drawBoundingSphereIfHit(tankBodyBound, 0);

		//Use your own draw code here to draw the rest of the tank
		//Here's the code for each individual part
		//Each part is placed with respect to the origin
		//you'll need to add in glPushMatrix/glTranslatef/glRotatef/glPopMatrix commands as necessary
		glPushMatrix();
			glRotatef(turretRot, 0.0, 1.0, 0.0);
			translateAndSavePos(0.0f, 15.0f, 0.0f);
			glColor3ub(115, 95, 68);
			glCallList(tankTurretLowList);
			addToTankPosList(tankPartPositions[1], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);
			BoundingSphere tankTurretBound = BoundingSphere(tankTurret, currGlobalPos);
			drawBoundingSphereIfHit(tankTurretBound, 1);
			//std::cout << "\nBoundingSphere center: " << tankTurretBound.center.x << ", " << tankTurretBound.center.y << ", " << tankTurretBound.center.z;



			glPushMatrix();
				translateAndSavePos(54.0f, -102.0f, 10.0f);
				glColor3ub(114, 90, 56);
				glCallList(tankMainGunLowList);
				addToTankPosList(tankPartPositions[2], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

				//std::cout << "\nTank main gun:\n Global pos: " << currGlobalPos.x << ", " << currGlobalPos.y << ", " << currGlobalPos.z;

				BoundingSphere tankMainGunBound = BoundingSphere(tankMainGun, currGlobalPos);
				drawBoundingSphereIfHit(tankMainGunBound, 2);

				translateAndSavePos(-54.0f, 102.0f, -10.0f);

			glPopMatrix();

			glPushMatrix();
				translateAndSavePos(-12.0f, 17.0f, -15.0f);
				glRotatef(secondaryGunRot, 0.0, 1.0, 0.0);
				translateAndSavePos(0.0f, 0.0f, 10.0f);
				glColor3ub(52, 46, 42);

				glCallList(tankSecondaryGunLowList);
				addToTankPosList(tankPartPositions[3], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

				BoundingSphere tankSecondaryGunBound = BoundingSphere(tankSecondaryGun, currGlobalPos);
				drawBoundingSphereIfHit(tankSecondaryGunBound, 3);


			glPopMatrix();

		glPopMatrix();

		glPushMatrix();
			glColor3ub(50, 38, 23);

			flipWheelRot = false;
			int zWheelPos = 43;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 20;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);
			zWheelPos -= 16;
			createWheel(-24, -10, zWheelPos);

			flipWheelRot = true;
			glRotatef(180, 0.0, 1.0, 0.0);

			zWheelPos = -43;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 20;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
			zWheelPos += 16;
			createWheel(-24.0f, -10.0, zWheelPos);
		glPopMatrix();

		//glColor3f(1.0f, 1.0f, 1.0f);

		boundingSpheres[0] = tankBodyBound;
		boundingSpheres[1] = tankTurretBound;
		boundingSpheres[2] = tankMainGunBound;
		boundingSpheres[3] = tankSecondaryGunBound;
		BoundingSphere fullTankBound = BoundingSphere(boundingSpheres);
		boundingSpheres[4] = fullTankBound;


	glPopMatrix();

}


void createWheel(float x, float y, float z)
{
	glPushMatrix();
		translateAndSavePos(x, y, z);
		glRotatef((flipWheelRot) ? wheelRot : -wheelRot, 1.0, 0.0, 0.0);
		if (zPos <= -300) {
			glCallList(tankWheelLowList);
		}
		else {
			glCallList(tankWheelList);
		}
	glPopMatrix();
}

void drawMesh(ObjMesh *pMesh, bool useTexture)
{
	//printf("num vertices: %i", pMesh->m_iNumberOfFaces);

	for (int i = 0; i < pMesh->m_iNumberOfFaces; i++) {
		ObjFace *pf = &pMesh->m_aFaces[i];
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++) {
			MyVector vertVec(pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].x, pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].y, pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].z);
			MyVector normVec(pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].x, pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].y, pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].z);

			if (useTexture) {
				MyVector texVec(pMesh->m_aTexCoordArray[pf->m_aTexCoordIndicies[j]].u, pMesh->m_aTexCoordArray[pf->m_aTexCoordIndicies[j]].v, 0);
				glTexCoord2f(texVec.x, texVec.y);
			}

			glNormal3f(normVec.x, normVec.y, normVec.z);
			glVertex3f(vertVec.x, vertVec.y, vertVec.z);
		}
		glEnd();
	}
}

// "shoots" a bullet at the current marker position to try the collison detection
void fireAway(void)
{
	// generate a random shot
	/*pos randomPos;
	randomPos.x = rand() % 199 + (-99);
	randomPos.y = rand() % 199 + (-99);
	randomPos.z = rand() % 199 + (-99);
	printf("\nRandom shot: %f, %f, %f", randomPos.x, randomPos.y, randomPos.z);*/

	printf("\n-------------------------------------\nShot at: %f, %f, %f", interactiveShotPos.x, interactiveShotPos.y, interactiveShotPos.z);

	int numSpheres = boundingSpheres.size();

	if (interactiveShotType == 2) {
		// check against line
		MyVector v = MyVector(interactiveShotPos.x, interactiveShotPos.y, interactiveShotPos.z);
		MyVector u = MyVector(1000.0, 0.0, 0.0);

		float dot = v.getDotProduct(u);

		float uLength = pow(u.getMagnitude(), 2);

		MyVector p = u * (dot / uLength);
		pos closestPoint = { p.x, p.y, p.z };
	}
	else {
		// check against point
		pos closestPoint = interactiveShotPos;
	}

	// let's check the big bounding sphere first
	if (boundingSpheres[4].inBounds(interactiveShotPos)) {
		// we have a potential hit on the tank
		printf("\nWe have a possible hit on the tank\n");

		// let's check each individual part
		tankPartWasHit = false;
		for (int i = 0; i < (numSpheres - 1); i++)
		{
			if (boundingSpheres[i].inBounds(interactiveShotPos)) {
				tankPartWasHit = true;
				boundingSpheres[i].isHitByProjectile = true;

				// add the part "id" to the hitList
				hitList[i] = i;

				// print some info about the parts that were hit
				if (i == 0) {
					printf("Tank body was hit.\n  BoundingSphere position: %f, %f, %f\n  Radius: %f\n", boundingSpheres[i].globalPos.x, boundingSpheres[i].globalPos.y, boundingSpheres[i].globalPos.z, boundingSpheres[i].radius);
				}
				else if (i == 1) {
					printf("Tank turret was hit.\n  BoundingSphere position: %f, %f, %f\n  Radius: %f\n", boundingSpheres[i].globalPos.x, boundingSpheres[i].globalPos.y, boundingSpheres[i].globalPos.z, boundingSpheres[i].radius);
				}
				else if (i == 2) {
					printf("Tank main gun was hit.\n  BoundingSphere position: %f, %f, %f\n  Radius: %f\n", boundingSpheres[i].globalPos.x, boundingSpheres[i].globalPos.y, boundingSpheres[i].globalPos.z, boundingSpheres[i].radius);
				}
				else if (i == 3) {
					printf("Tank secondary gun was hit.\n  BoundingSphere position: %f, %f, %f\n  Radius: %f\n", boundingSpheres[i].globalPos.x, boundingSpheres[i].globalPos.y, boundingSpheres[i].globalPos.z, boundingSpheres[i].radius);
				}
			}
			else {
				boundingSpheres[i].isHitByProjectile = false;
				hitList[i] = -1;
			}
		}
		// no tank part was actually hit
		if (!tankPartWasHit) {
			printf("Just kidding, it missed all parts!");
		}
	}
	else {
		printf("\nThe shot did not hit the tank.");
		// reset the hitList
		hitList[0] = hitList[1] = hitList[2] = hitList[3] = -1;
	}	
}

void drawBoundingSphereIfHit(BoundingSphere bs, int i)
{
	if (hitList[i] == i) {
		// the part is in the hitList
		//printf("\nTank part %i was hit.", i);
		glPushMatrix();
			//printf("\nDrawing boundingSphere at: %f, %f, %f", boundingSpheres[i].globalPos.x, boundingSpheres[i].globalPos.y, boundingSpheres[i].globalPos.z);
			boundingSpheres[i].drawBoundingSphere();
		glPopMatrix();
	}
}

// draw a sphere to mark where the shot will hit
void drawInteractiveShot(float x, float y, float z)
{
	//
	glPushMatrix();
		translateAndSavePos(x, y, z);
		glColor3f(1.0, 1.0, 1.0);
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(interactiveShotPos.x, interactiveShotPos.y, interactiveShotPos.z);

		// draw sphere or line
		if (interactiveShotType == 1) {
			glutSolidSphere(3, 30, 30);
		}
		else {
			glLineWidth(2.5);
			glBegin(GL_LINES);
				glVertex3f(-1000, 0.0, 0.0);
				glVertex3f(1000, 0, 0);
				glEnd();
			glLineWidth(1.0);
		}

	glPopMatrix();
}

//draw callback function - this is called by glut whenever the 
//window needs to be redrawn
void draw(void)
{
  //clear the current window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //make changes to the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  //initialise the modelview matrix to the identity matrix
  glLoadIdentity();

  // reset the global position variables
  currGlobalPos.x = 0.0;
  currGlobalPos.y = 0.0;
  currGlobalPos.z = 0.0;

  glPushMatrix();

	  glTranslatef(0.0,0.0,zPos);

	  glRotatef(yRot,0.0,1.0,0.0);


	  //draw the tank on screen at a position
	  if (zPos <= -300) {
		  draw_tank_low(0.0, -3.0, 0.0);
	  }
	  else {
		  draw_tank(0.0, -3.0, 0.0);
	  }

	  drawInteractiveShot(0.0, -3.0, 0.0);

	  // ---------------------- PARTICLE STUFF ------------------- //
	  if (tankPartWasHit){
		  glPushMatrix();
			  glEnable(GL_DEPTH_TEST);
			  glDepthMask(GL_FALSE);

			  // move the particles to the center of the projectile and scale properly
			  glTranslatef(0.0, -3.0, 0.0);
			  glScalef(0.1, 0.1, 0.1);

			  glEnable(GL_TEXTURE_2D);
			  glEnable(GL_BLEND);

			  //specify blending function here using glBlendFunc
			  glBlendFunc(GL_ONE, GL_ONE);

			  g_pParticleSystems[g_nActiveSystem]->SetPosition(MyVector(interactiveShotPos.x, interactiveShotPos.y, interactiveShotPos.z));
			  //glBindTexture(GL_TEXTURE_2D, g_pParticleSystems[g_nActiveSystem]->GetTextureID()); // this makes everything white, because of reasons..
			  g_pParticleSystems[g_nActiveSystem]->Render();

			  //
			  // Reset OpenGL states...
			  //
			  glDepthMask(GL_TRUE);
			  glDisable(GL_BLEND);
		  glPopMatrix();
	  }
	  // ---------------------- END PARTICLE STUFF ------------------- //

	  //drawHitBoundingSpheres(0.0, -3.0, 0.0);

  glPopMatrix();
  //flush what we've drawn to the buffer
  glFlush();
  //swap the back buffer with the front buffer
  glutSwapBuffers();
}

//idle callback function - this is called when there is nothing 
//else to do
void idle(void)
{
	// draw the sparks if the last shot hit the tank
	if (tankPartWasHit) {
	
		g_dCurTime = timeGetTime();
		g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
		g_dLastTime = g_dCurTime;

		g_pParticleSystems[g_nActiveSystem]->Update((float)g_fElpasedTime);

		// keep animating
		glutPostRedisplay();
	}
}

//key callback function - called whenever the user presses a 
//key
void key(unsigned char k, int x, int y)
{
  switch(k)
  {
    
	case 'q':
		turretRot--;
		break;
	case 'e':
		turretRot++;
		break;
	case 'x':
		secondaryGunRot--;
		break;
	case 'c':
		secondaryGunRot++;
		break;
	case 'w':
		zPos++;
		break;
	case 's':
		zPos--;
		break;
	case 'a':
		yRot++;
		break;
	case 'd':
		yRot--;
		break;
	case 'f':
		wheelRot += 5.0;
		break;
	case 'r':
		wheelRot -= 5.0;
		break;
	case 'y':
		interactiveShotPos.x -= 5.0;
		tankPartWasHit = false;
		break;
	case 'u':
		interactiveShotPos.x += 5.0;
		tankPartWasHit = false;
		break;
	case 'h':
		interactiveShotPos.y -= 5.0;
		tankPartWasHit = false;
		break;
	case 'j':
		interactiveShotPos.y += 5.0;
		tankPartWasHit = false;
		break;
	case 'n':
		interactiveShotPos.z -= 5.0;
		tankPartWasHit = false;
		break;
	case 'm':
		interactiveShotPos.z += 5.0;
		tankPartWasHit = false;
		break;
    case 27: //27 is the ASCII code for the ESCAPE key
		exit(0);
		break;
	case 32: // 32 is the ASCII code for the SPACE BAR
		fireAway();
		break;
	case '1':
		interactiveShotType = 1;
		break;
	case '2':
		interactiveShotType = 2;
		break;
  }
  glutPostRedisplay();
}

//reshape callback function - called when the window size changed
void reshape(int width, int height)
{
  //set the viewport to be the same width and height as the window
  glViewport(0,0,width, height);
  //make changes to the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //set up our projection type
  //we'll be using a perspective projection, with a 90 degree 
  //field of view
  gluPerspective(45.0, (float) width / (float) height, 1.0, 1000.0);
  //redraw the view during resizing
  draw();
}

//set up OpenGL before we do any drawing
//this function is only called once at the start of the program
void init_drawing(void)
{
  //blend colours across the surface of the polygons
  //another mode to try is GL_FLAT which is flat shading
  glShadeModel(GL_FLAT);
  //turn lighting off
  glDisable(GL_LIGHTING);
  //enable OpenGL hidden surface removal
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  GLfloat specular[] = {0.2,0.2,0.2,1.0};
  GLfloat ambient[] = {1.0,1.0,1.0,1.0};
  GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
  GLfloat position[] = {0.0,30.0,0.0,1.0};
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);

  GLfloat position1[] = {10.0,30.0,0.0,1.0};
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);



  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_2D);
}

void addToTankPosList(vec4 &vec, float x, float y, float z, float s) {
	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.s = s;
}

void initParticles(void)
{
	g_dCurTime = timeGetTime();
	g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
	g_dLastTime = g_dCurTime;


	//
	// Exploding burst
	//

	g_pParticleSystems[0] = new CParticleSystem();

	//g_pParticleSystems[0]->SetTexture( "..\\particle.bmp" );
	g_pParticleSystems[0]->SetTexture("particle.bmp");
	g_pParticleSystems[0]->SetMaxParticles(100);
	g_pParticleSystems[0]->SetNumToRelease(100);
	g_pParticleSystems[0]->SetReleaseInterval(0.05f);
	g_pParticleSystems[0]->SetLifeCycle(0.5f);
	g_pParticleSystems[0]->SetSize(10.0f);
	g_pParticleSystems[0]->SetColor(MyVector(1.0f, 0.5f, 0.0f));
	//g_pParticleSystems[0]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	//g_pParticleSystems[0]->SetVelocity(MyVector(15.0f, 15.0f, 15.0f));
	g_pParticleSystems[0]->SetGravity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[0]->SetWind(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[0]->SetVelocityVar(50.0f);

	g_pParticleSystems[0]->Init();

	//
	// Wind blown fountain
	//

	g_pParticleSystems[1] = new CParticleSystem();

	//    g_pParticleSystems[1]->SetTexture( "..\\particle.bmp" );
	g_pParticleSystems[1]->SetTexture("particle.bmp");
	g_pParticleSystems[1]->SetMaxParticles(500);
	g_pParticleSystems[1]->SetNumToRelease(5);
	g_pParticleSystems[1]->SetReleaseInterval(0.05f);
	g_pParticleSystems[1]->SetLifeCycle(4.0f);
	g_pParticleSystems[1]->SetSize(30.0f);
	g_pParticleSystems[1]->SetColor(MyVector(1.0f, 1.0f, 1.0f));
	g_pParticleSystems[1]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[1]->SetVelocity(MyVector(0.0f, 5.0f, 0.0f));
	g_pParticleSystems[1]->SetGravity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[1]->SetWind(MyVector(2.0f, 0.0f, 0.0f));
	g_pParticleSystems[1]->SetVelocityVar(1.5f);

	g_pParticleSystems[1]->Init();

	//
	// Omni-directiional emission expanding into space with no air resistence
	//

	g_pParticleSystems[2] = new CParticleSystem();

	g_pParticleSystems[2]->SetTexture("particle.bmp");
	g_pParticleSystems[2]->SetMaxParticles(2048);
	g_pParticleSystems[2]->SetNumToRelease(10);
	g_pParticleSystems[2]->SetReleaseInterval(0.05f);
	g_pParticleSystems[2]->SetLifeCycle(5.0f);
	g_pParticleSystems[2]->SetSize(30.0f);
	g_pParticleSystems[2]->SetColor(MyVector(1.0f, 1.0f, 1.0f));
	g_pParticleSystems[2]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[2]->SetVelocity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[2]->SetGravity(MyVector(0.0f, 0.0f, 0.0f));

	g_pParticleSystems[2]->SetWind(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[2]->SetAirResistence(false);

	g_pParticleSystems[2]->SetVelocityVar(2.0f);

	g_pParticleSystems[2]->Init();

	//
	// Fountain particles behave like paint spots when striking a plane as 
	// the wind blowing them towards us
	//

	g_pParticleSystems[3] = new CParticleSystem();

	g_pParticleSystems[3]->SetTexture("particle.bmp");
	g_pParticleSystems[3]->SetMaxParticles(100);
	g_pParticleSystems[3]->SetNumToRelease(10);
	g_pParticleSystems[3]->SetReleaseInterval(0.05f);
	g_pParticleSystems[3]->SetLifeCycle(3.0f);
	g_pParticleSystems[3]->SetSize(30.0f);
	g_pParticleSystems[3]->SetColor(MyVector(1.0f, 1.0f, 1.0f));
	g_pParticleSystems[3]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[3]->SetVelocity(MyVector(0.0f, 5.0f, 0.0f));
	g_pParticleSystems[3]->SetGravity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[3]->SetWind(MyVector(0.0f, 0.0f, -20.0f));
	g_pParticleSystems[3]->SetVelocityVar(2.5f);

	g_pParticleSystems[3]->SetCollisionPlane(MyVector(0.0f, 0.0f, 1.0f),
		MyVector(0.0f, 0.0f, -5.0f),
		1.0f, CR_STICK);

	g_pParticleSystems[3]->Init();

	//
	// Fountain using a single collision plane acting as a floor
	//

	g_pParticleSystems[4] = new CParticleSystem();

	g_pParticleSystems[4]->SetTexture("particle.bmp");
	g_pParticleSystems[4]->SetMaxParticles(200);
	g_pParticleSystems[4]->SetNumToRelease(10);
	g_pParticleSystems[4]->SetReleaseInterval(0.05f);
	g_pParticleSystems[4]->SetLifeCycle(5.0f);
	g_pParticleSystems[4]->SetSize(30.0f);
	g_pParticleSystems[4]->SetColor(MyVector(1.0f, 1.0f, 1.0f));
	g_pParticleSystems[4]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[4]->SetVelocity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[4]->SetGravity(MyVector(0.0f, -9.8f, 0.0f));
	g_pParticleSystems[4]->SetWind(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[4]->SetVelocityVar(20.0f);

	g_pParticleSystems[4]->SetCollisionPlane(MyVector(0.0f, 1.0f, 0.0f),
		MyVector(0.0f, 0.0f, 0.0f));

	g_pParticleSystems[4]->Init();

	//
	// Fountain boxed-in by 6 collision planes
	//

	g_pParticleSystems[5] = new CParticleSystem();

	g_pParticleSystems[5]->SetTexture("particle.bmp");
	g_pParticleSystems[5]->SetMaxParticles(100);
	g_pParticleSystems[5]->SetNumToRelease(5);
	g_pParticleSystems[5]->SetReleaseInterval(0.05f);
	g_pParticleSystems[5]->SetLifeCycle(5.0f);
	g_pParticleSystems[5]->SetSize(30.0f);
	g_pParticleSystems[5]->SetColor(MyVector(1.0f, 1.0f, 1.0f));
	g_pParticleSystems[5]->SetPosition(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[5]->SetVelocity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[5]->SetGravity(MyVector(0.0f, -9.8f, 0.0f));
	g_pParticleSystems[5]->SetWind(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[5]->SetVelocityVar(20.0f);

	// Create a series of planes to collide with
	g_pParticleSystems[5]->SetCollisionPlane(MyVector(0.0f, 1.0f, 0.0f),
		MyVector(0.0f, 0.0f, 0.0f)); // Floor

	g_pParticleSystems[5]->SetCollisionPlane(MyVector(1.0f, 0.0f, 0.0f),
		MyVector(-3.0f, 0.0f, 0.0f)); // Left Wall

	g_pParticleSystems[5]->SetCollisionPlane(MyVector(-1.0f, 0.0f, 0.0f),
		MyVector(3.0f, 0.0f, 0.0f)); // Right Wall

	g_pParticleSystems[5]->SetCollisionPlane(MyVector(0.0f, 0.0f, 1.0f),
		MyVector(0.0f, 0.0f, -3.0f)); // Front Wall

	g_pParticleSystems[5]->SetCollisionPlane(MyVector(0.0f, 0.0f, -1.0f),
		MyVector(0.0f, 0.0f, 3.0f)); // Back Wall

	g_pParticleSystems[5]->SetCollisionPlane(MyVector(0.0f, -1.0f, 0.0f),
		MyVector(0.0f, 5.0f, 0.0f)); // Ceiling

	g_pParticleSystems[5]->Init();


	//
	// Custom fountain thingy
	//

	g_pParticleSystems[6] = new CParticleSystem();

	g_pParticleSystems[6]->SetTexture("particle.bmp");
	g_pParticleSystems[6]->SetMaxParticles(100);
	g_pParticleSystems[6]->SetNumToRelease(5);
	g_pParticleSystems[6]->SetReleaseInterval(1.0f);
	g_pParticleSystems[6]->SetLifeCycle(20.0f);
	g_pParticleSystems[6]->SetSize(30.0f);
	g_pParticleSystems[6]->SetColor(MyVector(1.0f, 0.5f, 0.0f));
	g_pParticleSystems[6]->SetPosition(MyVector(0.0f, 5.0f, 0.0f));
	g_pParticleSystems[6]->SetVelocity(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[6]->SetGravity(MyVector(0.0f, -9.8f, 0.0f));
	g_pParticleSystems[6]->SetWind(MyVector(0.0f, 0.0f, 0.0f));
	g_pParticleSystems[6]->SetVelocityVar(15.0f);

	// Create a series of planes to collide with
	g_pParticleSystems[6]->SetCollisionPlane(MyVector(0.0f, 1.0f, 0.0f),
		MyVector(0.0f, -5.0f, 0.0f)); // Floor

	g_pParticleSystems[6]->SetCollisionPlane(MyVector(1.0f, 0.0f, 0.0f),
		MyVector(-5.0f, 0.0f, 0.0f), 1, CR_STICK); // Left Wall

	g_pParticleSystems[6]->SetCollisionPlane(MyVector(-1.0f, 0.0f, 0.0f),
		MyVector(5.0f, 0.0f, 0.0f), 1, CR_STICK); // Right Wall

	g_pParticleSystems[6]->SetCollisionPlane(MyVector(0.0f, 0.0f, 1.0f),
		MyVector(0.0f, 0.0f, -1.0f)); // Front Wall

	g_pParticleSystems[6]->SetCollisionPlane(MyVector(0.0f, 0.0f, -1.0f),
		MyVector(0.0f, 0.0f, 1.0f)); // Back Wall

	g_pParticleSystems[6]->SetCollisionPlane(MyVector(0.0f, -1.0f, 0.0f),
		MyVector(0.0f, 5.0f, 0.0f)); // Ceiling

	g_pParticleSystems[6]->Init();
}

//#endif /* CPARTICLESYSTEM_H_INCLUDED */
#endif /* MYVECTOR_AND_MYMATRIX_H_INCLUDED */