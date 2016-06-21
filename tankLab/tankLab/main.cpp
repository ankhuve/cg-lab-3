#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"
#include <iostream>

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "myvector.h"
#include "mymatrix.h"
using namespace MyMathLibrary;

#include "stdlib.h"
#include "stdio.h"

#include "objloader.h"

#include "BoundingSphere.h"

ObjMesh* tankBody;
ObjMesh* tankTurret;
ObjMesh* tankMainGun;
ObjMesh* tankSecondaryGun;
ObjMesh* tankWheel;

void load_tank_objs(void);
float zPos = -30.0;
float yRot = 0.0;

float turretRot = 0.0;
float secondaryGunRot = 0.0;
float wheelRot = 0.0;
bool flipWheelRot = false;
Position currGlobalPos;


vec4 tankPartPositions[4];


GLuint tankBodyList, tankTurretList, tankMainGunList, tankSecondaryGunList, tankWheelList;

//prototypes for our callback functions
void draw(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation
void createWheel(float x, float y, float z);
void drawMesh(ObjMesh *pMesh);
void addToTankPosList(vec4 vec, float x, float y, float z, float s);

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

  //run our own drawing initialisation routine
  init_drawing();

  load_tank_objs();


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



struct vec4 {
	float x, y, z, s;
};


void load_tank_objs(void)
{
  tankBody = LoadOBJ(".\\tankobjs\\tankbody.obj");
  tankTurret = LoadOBJ(".\\tankobjs\\tankturret.obj");
  tankMainGun = LoadOBJ(".\\tankobjs\\tankmaingun.obj");
  tankSecondaryGun = LoadOBJ(".\\tankobjs\\tanksecondarygun.obj");
  tankWheel = LoadOBJ(".\\tankobjs\\tankwheel.obj");
  SetTextures(tankBody->m_iMeshID, NULL, ".\\tankobjs\\texture.tga");

  tankBodyList = glGenLists(5);
  tankTurretList = tankBodyList + 1;
  tankMainGunList = tankTurretList + 1;
  tankSecondaryGunList = tankMainGunList + 1;
  tankWheelList = tankSecondaryGunList + 1;

  glNewList(tankBodyList, GL_COMPILE);
	//DrawOBJ(tankBody->m_iMeshID);
	drawMesh(tankBody);
  glEndList();

  glNewList(tankTurretList, GL_COMPILE);
	//DrawOBJ(tankTurret->m_iMeshID);
	drawMesh(tankTurret);
  glEndList();

  glNewList(tankMainGunList, GL_COMPILE);
	//DrawOBJ(tankMainGun->m_iMeshID);
	drawMesh(tankMainGun);
  glEndList();

  glNewList(tankSecondaryGunList, GL_COMPILE);
	//DrawOBJ(tankSecondaryGun->m_iMeshID);
	drawMesh(tankSecondaryGun);
  glEndList();

  glNewList(tankWheelList, GL_COMPILE);
	//DrawOBJ(tankWheel->m_iMeshID);
	drawMesh(tankWheel);
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
	translateAndSavePos(x,y,z);


	glScalef(0.1,0.1,0.1);		//reduce the size of the tank on screen
	glCallList(tankBodyList);
	addToTankPosList(tankPartPositions[0], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);
	
	//Use your own draw code here to draw the rest of the tank
	//Here's the code for each individual part
	//Each part is placed with respect to the origin
	//you'll need to add in glPushMatrix/glTranslatef/glRotatef/glPopMatrix commands as necessary
	glPushMatrix();
		glRotatef(turretRot, 0.0, 1.0, 0.0);
		translateAndSavePos(0.0f, 15.0f, 0.0f);
		glCallList(tankTurretList);
		addToTankPosList(tankPartPositions[1], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

		

		glPushMatrix();
		translateAndSavePos(54.0f, -102.0f, 10.0f);
			glCallList(tankMainGunList);
			addToTankPosList(tankPartPositions[2], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);

		glPopMatrix();

		glPushMatrix();
		
		translateAndSavePos(-12.0f, 17.0f, -15.0f);
		glRotatef(secondaryGunRot, 0.0, 1.0, 0.0);
		translateAndSavePos(0.0f, 0.0f, 10.0f);
		glCallList(tankSecondaryGunList);
		addToTankPosList(tankPartPositions[3], currGlobalPos.x, currGlobalPos.y, currGlobalPos.z, 0.1);


		glPopMatrix();

	glPopMatrix();

	

	glPushMatrix();
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


	glPopMatrix();
}

void createWheel(float x, float y, float z)
{
	glPushMatrix();
	
	translateAndSavePos(x, y, z);
	glRotatef((flipWheelRot) ? wheelRot : -wheelRot, 1.0, 0.0, 0.0);
	glCallList(tankWheelList);
	glPopMatrix();
}

void drawMesh(ObjMesh *pMesh)
{
	for (int i = 0; i < pMesh->m_iNumberOfFaces; i++) {
		ObjFace *pf = &pMesh->m_aFaces[i];
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++) {
			MyVector vertVec(pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].x, pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].y, pMesh->m_aVertexArray[pf->m_aVertexIndices[j]].z);
			MyVector normVec(pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].x, pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].y, pMesh->m_aNormalArray[pf->m_aNormalIndices[j]].z);
			MyVector texVec(pMesh->m_aTexCoordArray[pf->m_aTexCoordIndicies[j]].u, pMesh->m_aTexCoordArray[pf->m_aTexCoordIndicies[j]].v, 0);
			glTexCoord2f(texVec.x, texVec.y);
			glNormal3f(normVec.x, normVec.y, normVec.z);
			glVertex3f(vertVec.x, vertVec.y, vertVec.z);
		}
		glEnd();
	}
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

  translateAndSavePos(0.0,0.0,zPos);

  glRotatef(yRot,0.0,1.0,0.0);

  //draw the tank on screen at a position
  draw_tank(0.0, -3.0, 0.0);
  BoundingSphere tankBodyBound(tankBody);
  BoundingSphere tankMainGunBound(tankMainGun);
  BoundingSphere tankTurretBound(tankTurret);
  BoundingSphere tankSecondaryGunBound(tankSecondaryGun);
  BoundingSphere tankWheelBound(tankWheel);

  //flush what we've drawn to the buffer
  glFlush();
  //swap the back buffer with the front buffer
  glutSwapBuffers();
}

//idle callback function - this is called when there is nothing 
//else to do
void idle(void)
{
  //this is a good place to do animation
  //since there are no animations in this test, we can leave 
  //idle() empty
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
    case 27: //27 is the ASCII code for the ESCAPE key
		exit(0);
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
  gluPerspective(45.0, (float) width / (float) height, 1.0, 100.0);
  //redraw the view during resizing
  draw();
}

//set up OpenGL before we do any drawing
//this function is only called once at the start of the program
void init_drawing(void)
{
  //blend colours across the surface of the polygons
  //another mode to try is GL_FLAT which is flat shading
  glShadeModel(GL_SMOOTH);
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

void addToTankPosList(vec4 vec, float x, float y, float z, float s) {
	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.s = s;
}