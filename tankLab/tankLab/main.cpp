#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "myvector.h"
#include "mymatrix.h"
using namespace MyMathLibrary;

#include "stdlib.h"
#include "stdio.h"

#include "objloader.h"

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

GLuint tankBodyList, tankTurretList, tankMainGunList, tankSecondaryGunList, tankWheelList;

//prototypes for our callback functions
void draw(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation
void createWheel(float x, float y, float z);
void drawMesh(ObjMesh *pMesh);

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
	DrawOBJ(tankBody->m_iMeshID);
  glEndList();

  glNewList(tankTurretList, GL_COMPILE);
	DrawOBJ(tankTurret->m_iMeshID);
  glEndList();

  glNewList(tankMainGunList, GL_COMPILE);
	DrawOBJ(tankMainGun->m_iMeshID);
  glEndList();

  glNewList(tankSecondaryGunList, GL_COMPILE);
	DrawOBJ(tankSecondaryGun->m_iMeshID);
  glEndList();

  glNewList(tankWheelList, GL_COMPILE);
	DrawOBJ(tankWheel->m_iMeshID);
  glEndList();


}

void draw_tank(float x, float y, float z)
{
	glRotatef(-90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(x,y,z);


	glScalef(0.1,0.1,0.1);		//reduce the size of the tank on screen
	glCallList(tankBodyList);
	
	//Use your own draw code here to draw the rest of the tank
	//Here's the code for each individual part
	//Each part is placed with respect to the origin
	//you'll need to add in glPushMatrix/glTranslatef/glRotatef/glPopMatrix commands as necessary
	glPushMatrix();
		glRotatef(turretRot, 0.0, 1.0, 0.0);
		glTranslatef(0.0f, 15.0f, 0.0f);
		glCallList(tankTurretList);
		

		glPushMatrix();
		glTranslatef(54.0f, -102.0f, 10.0f);
		glCallList(tankMainGunList);
		glPopMatrix();

		glPushMatrix();
		
		glTranslatef(-12.0f, 17.0f, -15.0f);
		glRotatef(secondaryGunRot, 0.0, 1.0, 0.0);
		glTranslatef(0.0f, 0.0f, 10.0f);
		glCallList(tankSecondaryGunList);

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
	
	glTranslatef(x, y, z);
	glRotatef((flipWheelRot) ? wheelRot : -wheelRot, 1.0, 0.0, 0.0);
	glCallList(tankWheelList);
	glPopMatrix();
}

void drawMesh(ObjMesh *pMesh)
{
	int numFaces = pMesh->m_iNumberOfFaces;
	for (int i = 0; i < numFaces; i++) {
		ObjFace *pf = &pMesh->m_aFaces[i];
		for (int j = 0; j < 3; j++) {
			int k = pf->m_aVertexIndices[j];
		}
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

  glTranslatef(0.0,0.0,zPos);

  glRotatef(yRot,0.0,1.0,0.0);

  //draw the tank on screen at a position
  draw_tank(0.0, -3.0, 0.0);

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