#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>



//for cut the sphere (used for fishe's eyes)
GLdouble eqn2[4] = { 0.0, 0.0, 0.0, 0.0 };

//used for rotation of objects (boat,sun,moon,stars,ship,cloud and fish)
GLfloat boatAngle, boatFlip = 0, sunAngle, fishAngle = 0, moonAngle = 0, starAngle = 0, shipAngle, cloudAngle = 0;

//used for changing direction of movement into the clockwise or nonclockwise
int dirFlag = 0;      

//esharegar be fazayi ke ye object quadric mikhad begire
GLUquadricObj * quadric; 

//angle= camera angle |  deltaangle=camera alternation  | ratio= w / h
float angle = 0.0, deltaAngle = 0.0, ratio;  

//fisrt position of camera
float x = -4.0f, y = 3, z = 20.0f; 

//horizon
float lx = 0.0f, ly = 0.0f, lz = -5.0f;  

// step of camera motion
int deltaMove = 0, h, w;  

// address of beginning the list
static GLint Fish_Display_List; 

//time= present time , timebase=time origin
int frame, time, timebase = 0;  

//for movement of ship and fish
float v, f, F = 0;   

//ambientLight
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };   

//diffuseLight
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 

//specular
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  

//used for set position of lighting
GLfloat  position[] = { 4,2.5,-5,1.0 };

//used for choosing day and night moods
int glob = 0;


void init(void) {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	GLfloat yellow[] = { 1.0, 1.0, 0.5, 1.0 };
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);    
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

}

//draw circle using cmath class
void drawCircle(float x, float y, float z, float r)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);
	for (float angle = 0;angle<2.0f*3.141592f;angle += 0.1f)
		glVertex3f(x + (float)cos(angle)*r, y + (float)sin(angle)*r, z);
	glVertex3f(x + r, y, z);
	glEnd();
}

// Prevent a divide by zero, when window is too short (you cant make a window of zero width).
void changeSize(int w1, int h1)
{
	if (h1 == 0)
		h1 = 1;
	w = w1;
	h = h1;
	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the clipping volume
	gluPerspective(45, ratio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z,                          
		x + lx, y + ly, z + lz,              
		0.0f, 1.0f, 1.0f);                        
}

//menu for choosing between night and day or exit
void mainmenu(int id)
{
	switch (id)
	{
	case 1:exit(0);
		break;
	case 2:glob = 0;
		break;
	case 3:glob = 1;
		break;

	}
	glutPostRedisplay();
}

//fish contain 5 spheres for head and eyes and two cones for tail and whale
void drawFish() {

	//head
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.30f, 0.0f);
	glTranslatef(0.1f, 0.50f, 0.0f);
	glutSolidSphere(0.1, 20, 20);    
	glPopMatrix();
	
	//left eye
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(-0.001, 0.77, 0.07);
	glClipPlane(GL_CLIP_PLANE0, eqn2);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.027, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//right eye
	glPushMatrix();
	glColor3f(1.07, 1.0, 1.0);
	glTranslatef(-0.001, 0.77, -0.07);
	glClipPlane(GL_CLIP_PLANE0, eqn2);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.027, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//right pupil
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.03, 0.77, -0.07);
	glClipPlane(GL_CLIP_PLANE0, eqn2);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.017, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//left pupil
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.03, 0.77, 0.07);
	glClipPlane(GL_CLIP_PLANE0, eqn2);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.017, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//tail
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0.4f, 0.8f, 0.0f);
	glRotatef(180, -6.0, 0.0, 6.0);
	glutSolidCone(0.10f, 0.31f, 20, 20);     
	glPopMatrix();

	// whale
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0.14, 0.74, 0);
	glRotatef(100, -180, 150, 150);
	glScalef(0.1, 1.0, 1.0);
	glutSolidCone(0.15, 0.26, 20, 20);
	glPopMatrix();

}

//fish contain 5 spheres for head and eyes and two cones for tail and whale
void drawOrangeFish() {
	glPushMatrix();
	glTranslatef(0, 0, F);
	{
		glPushMatrix();
		glRotatef(fishAngle, 0, 1, 1);
		//head
		glColor3f(1.000, 0.271, 0.000);
		glPushMatrix();
		glTranslatef(0.0f, 0.30f, 0.0f);
		glTranslatef(0.1f, 0.50f, 0.0f);
		glutSolidSphere(0.1, 20, 20);     
		glPopMatrix();

		//left eye
		glPushMatrix();
		glColor3f(1.07, 1.0, 1.0);
		glTranslatef(-0.001, 0.77, 0.07);
		glClipPlane(GL_CLIP_PLANE0, eqn2);
		glEnable(GL_CLIP_PLANE0);
		glutSolidSphere(0.027, 20, 20);
		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//right eye
		glPushMatrix();
		glColor3f(1.07, 1.0, 1.0);
		glTranslatef(-0.001, 0.77, -0.07);
		glClipPlane(GL_CLIP_PLANE0, eqn2);
		glEnable(GL_CLIP_PLANE0);
		glutSolidSphere(0.027, 20, 20);
		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//right pupil
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-0.03, 0.77, -0.07);
		glClipPlane(GL_CLIP_PLANE0, eqn2);
		glEnable(GL_CLIP_PLANE0);
		glutSolidSphere(0.017, 20, 20);
		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//left pupil
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(-0.03, 0.77, 0.07);
		glClipPlane(GL_CLIP_PLANE0, eqn2);
		glEnable(GL_CLIP_PLANE0);
		glutSolidSphere(0.017, 20, 20);
		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//tail
		glPushMatrix();
		glColor3f(1.000, 0.271, 0.000);
		glTranslatef(0.4f, 0.8f, 0.0f);
		glRotatef(180, -6.0, 0.0, 6.0);
		glutSolidCone(0.10f, 0.31f, 20, 20);     
		glPopMatrix();

		// whale
		glPushMatrix();
		glColor3f(1.000, 0.271, 0.000);
		glTranslatef(0.14, 0.74, 0);
		glRotatef(90, -150, 120, 120);
		glScalef(0.1, 1.0, 1.0);
		glutSolidCone(0.15, 0.26, 20, 20);
		glPopMatrix();
		glPopMatrix();
	}
}

//sun has made of one sphere and 6 triangles
void DrawSun()
{
	glPushMatrix();
	glTranslatef(-3.0, 3.2, 0.0);
	glColor3f(1, 1, 0.0);
	glutSolidSphere(0.5, 20, 20);
	glRotatef(sunAngle, 0.0, 0.0, 1.0);

	glColor3f(1.000, 0.843, 0.000);
	for (int i = 0; i <= 5; i++) {
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(0.2, 0.4, 0.0);
		glVertex3f(0.0, 0.9, 0.0);
		glVertex3f(-0.2, 0.4, 0.0);
		glEnd();
		glRotatef(60.0, 0.0, 0.0, 1.0);
	}
	glPopMatrix();
}

//drawing sea using polygon
void DrawSea()
{
	glBegin(GL_POLYGON);
	glColor3f(0.000, 0.000, 0.804);
	glVertex3f(63.0, -0.1, -63.0);
	glVertex3f(-63.0, -0.1, -63.0);
	glVertex3f(-63.0, -0.1, 63.0);
	glVertex3f(63.0, -0.1, 63.0);
	glEnd();
}

//drawing sea using polygon
void DrawSeaAtNight()
{
	glBegin(GL_POLYGON);
	glColor3f(0.098, 0.098, 0.339);
	glVertex3f(63.0, -0.1, -63.0);
	glVertex3f(-63.0, -0.1, -63.0);
	glVertex3f(-63.0, -0.1, 63.0);
	glVertex3f(63.0, -0.1, 63.0);
	glEnd();
}

//moon has made of one wire sphere 
void DrawMoon() {
	
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-4.0, 4.2, 0.0);
	glRotatef(moonAngle, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	glutWireSphere(0.9, 20, 20);	
	glPopMatrix();
}

//boat has made of two cubes for body ,a cone for sail and some polygons to complete it:D
void DrawBoat()
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 10.9);
	glRotatef(boatAngle, 0.0, 1.0, 0.0);	
	glTranslatef(1.0, -0.1, 0.0);
	glRotatef(boatFlip, 0.0, 1.0, 0.0);

	//body
	glPushMatrix();
	glColor3ub(91, 46, 7);
	glTranslatef(0.0, 0.3, 0.0);
	glScalef(0.7, 8.0, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	//sail
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.0, 0.2, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(0.1, 1.0, 1.0);
	glutSolidCone(0.3, 0.5, 20, 20);
	glPopMatrix();

	//shaft
	glColor3ub(91, 46, 7);
	glPushMatrix();
	glScalef(0.5, 0.5, 1.0);
	glutSolidCube(0.5);
	glPopMatrix();

	//behind
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.25);
	glScalef(0.3, 2.0, 2.0);
	glutSolidCube(0.1);
	glPopMatrix();

	// front
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.125, 0.125, -0.25);
	glVertex3f(0.125, 0.125, -0.25);
	glVertex3f(0.0, 0.125, -0.5);

	glNormal3f(-0.1875, -0.1875, -0.03125);
	glVertex3f(-0.125, 0.125, -0.25);
	glVertex3f(0.0, 0.125, -0.5);
	glVertex3f(-0.125, -0.125, -0.25);

	glNormal3f(0.1875, -0.1875, -0.03125);
	glVertex3f(0.125, 0.125, -0.25);
	glVertex3f(0.0, 0.125, -0.5);
	glVertex3f(0.125, -0.125, -0.25);

	glNormal3f(0.0, -0.5, -0.5);
	glVertex3f(0.125, -0.125, -0.25);
	glVertex3f(-0.125, -0.125, -0.25);
	glVertex3f(0.0, 0.125, -0.5);
	glEnd();
	glPopMatrix();
}

//ship has made of 4 cubes for body and some spheres for exhaust
void drawship() {
	glPushMatrix();
	glRotatef(shipAngle, 0, 0, 0);
	glTranslatef(f, 0, v);
	{
		glPushMatrix();
		{
			//body
			glTranslatef(-3, .2, 2);
			glPushMatrix();
			glColor3f(0.545, 0.271, 0.075);
			glScalef(2, 0.25, 1);
			glutSolidCube(1.3);
			glPopMatrix();


			glPushMatrix();
			glColor3f(0.545, 0.271, 0.075);
			glScalef(1, .5, .5);
			glutSolidCube(1.1);
			glPopMatrix();

			glTranslatef(0, 0.2, 0);
			glPushMatrix();
			glColor3f(1.000, 0.871, 0.678);
			glScalef(1, .5, .5);
			glutSolidCube(0.5);
			glPopMatrix();

			glTranslatef(0, 0.17, 0);
			glPushMatrix();
			glColor3f(1.000, 0.871, 0.678);
			glScalef(1, .8, .5);
			glutSolidCube(0.25);
			glPopMatrix();

			//exhaust
			glColor3f(0.663, 0.663, 0.663);
			glPushMatrix();
			glTranslatef(0, 0.67, -0.2);
			glutSolidSphere(0.2, 20, 20);   
			glPopMatrix();

			glColor3f(0.663, 0.663, 0.663);
			glPushMatrix();
			glTranslatef(0, 0.37, -0.05);
			glutSolidSphere(0.2, 20, 20);
			glPopMatrix();

			glColor3f(0.663, 0.663, 0.663);
			glPushMatrix();
			glTranslatef(0, 0.5, -0.4);
			glutSolidSphere(0.2, 20, 20);
			glPopMatrix();



		}
	}
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

//to draw many fishes from drawFish function
GLuint createDL() {
	GLuint Dl;
	// Create the id for the list
	Dl = glGenLists(2);
	glNewList(Dl + 1, GL_COMPILE);
	drawFish();
	glEndList();
	// start list
	glNewList(Dl, GL_COMPILE);
	// call the function that contains the rendering commands
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			glPushMatrix();
			if (i % 2 == 0 && j % 2 == 0)
				glTranslatef(i*-5.0, -.3, j * -11.0);
			else if (i % 2 != 0 && j % 2 == 0)
				glTranslatef(i*7.0, -.3, j * -4.0);
			else
				glTranslatef(i*11.0, -.3, j * 3.0);

			glCallList(Dl + 1);
			glPopMatrix();
		}
	}

	glEndList();
	return(Dl);
}

void initScene() {

	glEnable(GL_DEPTH_TEST);
	Fish_Display_List = createDL();
}

//rotation of camera
void orientMe(float ang) {

	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 4.0f, 0.0f);
}

//zoom in and zoom out
void moveMeFlat(int i) {

	x = x + i*(lx)*0.2;
	z = z + i*(lz)*0.2;
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

//draw objects when day mood choosed
void day() {

	//set sky color
	glClearColor(0.118, 0.565, 1.000, 0);

	//draw fishes
	glCallList(Fish_Display_List);
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		timebase = time;
		frame = 0;
	}

	//draw sun
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	DrawSun();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//cloud 1
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(8.3f, 7.7f, -15.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 2
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-9.4f, 7.1f, -15.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 3
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(cloudAngle, 0, 0, 0);
	glTranslatef(19.4f, 9.1f, -20.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 4
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(cloudAngle, 0, 0, 0);
	glTranslatef(-19.4f, 9.1f, -20.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 5
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(cloudAngle, 0, 0, 0);
	glTranslatef(0.4f, 6.1f, -25.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//rainbow
	glPushMatrix();
	glColor3f(0.545, 0.000, 0.545);
	glTranslatef(0.6, 8.5, -15.0);
	gluPartialDisk(quadric, 9.5, 10.5, 90, 1, -96.0, 190.0);
	glColor3f(0.000, 0.000, 0.804);
	glTranslatef(0.0, -0.5, 0.01);
	gluPartialDisk(quadric, 9.0, 10.2, 60, 1, -96.0, 190.0);
	glColor3f(1.000, 1.000, 0.000);
	glTranslatef(0.0, -0.5, 0.01);
	gluPartialDisk(quadric, 9.0, 9.8, 60, 1, -96.0, 190.0);
	glColor3f(1.000, 0.000, 0.000);
	glTranslatef(0.0, -0.5, 0.01);
	gluPartialDisk(quadric, 9.0, 9.6, 60, 1, -96.0, 190.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 15.0f, 0.0f);
	glPopMatrix();

	//draw orange fish 1
	glPushMatrix();
	glTranslatef(-5, 0, 3);
	drawOrangeFish();
	glPopMatrix();

	//draw orange fish 2
	glPushMatrix();
	glTranslatef(10, 0, 5);
	drawOrangeFish();
	glPopMatrix();
	glPopMatrix();

	//draw orange fish3
	glPushMatrix();
	glTranslatef(4, 0, -15);
	drawOrangeFish();
	glPopMatrix();
	glPopMatrix();

	//draw ship
	glPushMatrix();
	glTranslatef(13, 0, 0);
	glRotatef(0, 0, 1, 0);
	glScalef(1, 3, -5);
	drawship();
	glPopMatrix();

	//draw sea and boats
	glPushMatrix();
	glTranslatef(-5, 0.5, -17);
	DrawSea();
	DrawBoat();
	glPopMatrix();

	//draw boat number2
	glPushMatrix();
	glTranslatef(-15, 0.5, -20);
	DrawBoat();
	glPopMatrix();
	//draw boat number 3
	glPushMatrix();
	glTranslatef(0, 0.5, -15);
	DrawBoat();
	glPopMatrix();
	//draw boat number 4
	glPushMatrix();
	glTranslatef(16, 0.5, -15);
	DrawBoat();
	glPopMatrix();
}

//draw objects when night mood choosed
void night() {

	//set sky color
	glClearColor(0.000, 0.000, 0.000,0);

	//draw fishes
	glCallList(Fish_Display_List);
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		timebase = time;
		frame = 0;
	}

	//draw moon
	glPushMatrix();
	glTranslatef(position[0]-4, position[1]+3, position[2]-6);
	DrawMoon();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//cloud 1
	glPushMatrix();
	glColor3f(0.663, 0.663, 0.663);
	glRotatef(cloudAngle*-1, 0, 0, 0);
	glTranslatef(10.3f, 6.7f, -15.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 2
	glPushMatrix();
	glColor3f(0.663, 0.663, 0.663);
	glRotatef(cloudAngle*-1, 0, 0, 0);
	glTranslatef(-10.4f, 5.1f, -18.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 3
	glPushMatrix();
	glColor3f(0.663, 0.663, 0.663);
	glRotatef(cloudAngle*-1, 0, 0, 0);
	glTranslatef(17.4f, 9.1f, -20.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 4
	glPushMatrix();
	glColor3f(0.663, 0.663, 0.663);
	glRotatef(cloudAngle*-1, 0, 0, 0);
	glTranslatef(-20.4f, 7.1f, -22.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();

	//cloud 5
	glPushMatrix();
	glColor3f(0.663, 0.663, 0.663);
	glRotatef(cloudAngle*-1, 0, 0, 0);
	glTranslatef(0.4f, 6.1f, -25.0f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(1.1f, -0.1f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(0.9f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 40, 40);
	glTranslatef(0.2f, -1.13f, -0.1f);
	glutSolidSphere(0.8, 40, 40);
	glTranslatef(-1.0f, 0.2f, 0.1f);
	glutSolidSphere(0.6, 40, 40);
	glTranslatef(-1.2f, 0.4f, 0.1f);
	glutSolidSphere(0.8, 40, 40);
	glPopMatrix();
	
	//draw orange fish 1
	glPushMatrix();
	glTranslatef(-5, 0, 3);
	drawOrangeFish();
	glPopMatrix();

	//draw orange fish 2
	glPushMatrix();
	glTranslatef(10, 0, 5);
	drawOrangeFish();
	glPopMatrix();
	glPopMatrix();

	//draw orange fish3
	glPushMatrix();
	glTranslatef(4, 0, -15);
	drawOrangeFish();
	glPopMatrix();
	glPopMatrix();

	//draw ship
	glPushMatrix();
	glTranslatef(13, 0, 0);
	glRotatef(0, 0, 1, 0);
	glScalef(1, 3, -5);
	drawship();
	glPopMatrix();

	//draw sea and boats
	glPushMatrix();
	glTranslatef(-5, 0.5, -17);
	DrawSeaAtNight();
	DrawBoat();
	glPopMatrix();

	//draw boat number2
	glPushMatrix();
	glTranslatef(-15, 0.5, -20);
	DrawBoat();
	glPopMatrix();
	//draw boat number 3
	glPushMatrix();
	glTranslatef(0, 0.5, -15);
	DrawBoat();
	glPopMatrix();
	//draw boat number 4
	glPushMatrix();
	glTranslatef(16, 0.5, -15);
	DrawBoat();
	glPopMatrix();

	//draw star number 1
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(-1 * starAngle, 0, 0, 0);
	glTranslatef(10.0, 14, -20.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 2
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(-1 * starAngle, 0, 0, 0);
	glTranslatef(-10.0, 12, -20.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 3
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(starAngle, 0, 0, 0);
	glTranslatef(5.0, 16, -22.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 4
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(starAngle, 0, 0, 0);
	glTranslatef(-5.0, 14, -22.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 5
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(-1*starAngle, 0, 0, 0);
	glTranslatef(4.0, 7, -15.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 6
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(-1 * starAngle, 0, 0, 0);
	glTranslatef(17.0, 8, -20.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 7
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(-1 * starAngle, 0, 0, 0);
	glTranslatef(-14.0, 12, -23.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

	//draw star number 8
	glPushMatrix();
	glColor3f(0.512, 0.512, 0.512);
	glRotatef(starAngle, 0, 0, 0);
	glTranslatef(-10.0,6, -100.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidOctahedron();
	glPopMatrix();

}

void renderScene() {

	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle * 2);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glob == 0)
		day();
	else if (glob == 1) 
		night();
		

		setOrthographicProjection();
		glPushMatrix();
		glLoadIdentity();
		glPopMatrix();
		resetPerspectiveProjection();
		glutSwapBuffers();

}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.01f;
		break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.01f;
		break;
	case GLUT_KEY_UP: deltaMove = 1;
		break;
	case GLUT_KEY_DOWN: deltaMove = -1;
		break;
	}
}

void releaseKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: if (deltaAngle < 0.0f)
		deltaAngle = 0.0f;
		break;
	case GLUT_KEY_RIGHT: if (deltaAngle > 0.0f)
		deltaAngle = 0.0f;
		break;
	case GLUT_KEY_UP:	 if (deltaMove > 0)
		deltaMove = 0;
		break;
	case GLUT_KEY_DOWN: if (deltaMove < 0)
		deltaMove = 0;
		break;
	}
}

//function for maintaining rotation in an anti-clockwise direction
void aClockWise()
{
	if (sunAngle >= 360)
	{
		sunAngle = 0;
	}
	if (boatAngle >= 360)
	{
		boatAngle = 0;
	}

	if (fishAngle >= 360)
	{
		fishAngle = 0;
	}
	if (starAngle >= 45)
	{
		starAngle = starAngle* -1;
	}
	if (F >= 7 || F <= -7)
	{
		F = F*-1;
	}
	if (moonAngle >= 360) {

		moonAngle = 0;
	}
	if (shipAngle >= 30)
	{
		shipAngle = shipAngle*-1;
	}
	if (cloudAngle >= 30)
	{
		cloudAngle = cloudAngle*-1;
	}

	F += 0.01;
	cloudAngle += 0.5;
	shipAngle += 0.5;
	starAngle += 0.5;
	moonAngle += 0.5;
	fishAngle += 1;
	sunAngle += 0.5;
	boatAngle += 0.7;
	boatFlip = 0.0;
	glutPostRedisplay();
}

//function for maintaining rotation in a clockwise direction
void clockWise()
{
	F -= 0.01;
	cloudAngle -= 0.5;
	shipAngle -= 0.5;
	starAngle -= 0.5;
	moonAngle -= 0.5;
	fishAngle -= 1;
	sunAngle -= 0.5;
	boatAngle -= 0.7;
	boatFlip = 180.0;
	glutPostRedisplay();
}

void Keystroke(unsigned char key, int x, int y)
{

	if (key == 27)       	//"esc"
		exit(0);


	if (key == 'f')       //ship's move forward and backward
	{
		v = v + .15;
	}
	if (key == 'b')
	{
		v = v - .15;
	}

	if (key == 'r')       //ship's move right and left
	{
		f = f + .15;
	}
	if (key == 'l')
	{
		f = f - .15;
	}

	if (key == 'o')       //ship's move diagonal to right and forward
	{
		f = f + .3;
		v = v + .3;
	}
	if (key == 'O')		//ship's move diagonal to  left and backward 
	{
		f = f - .3;
		v = v - .3;
	}

	if (key == 'i')       //ship's move diagonal to right and backward
	{
		f = f + .3;
		v = v - .3;
	}
	if (key == 'I')		//ship's move diagonal to  left and forward
	{
		f = f - .3;
		v = v + .3;
	}


	if (key == 'p')          	//"p" = stop everything
		glutIdleFunc(NULL);

	if (key == 's') {            //"s" resume in proper direction
		if (dirFlag == 0)
			glutIdleFunc(aClockWise);
		else
			glutIdleFunc(clockWise);
	}

	if (key == 'c') {        		//"c" change direction of rotation
		if (dirFlag == 0)
		{
			dirFlag = 1;
			glutIdleFunc(clockWise);
		}
		else
		{
			dirFlag = 0;
			glutIdleFunc(aClockWise);
		}
	}


}

void initWindow() {
	glutIgnoreKeyRepeat(0);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(aClockWise);   /*Register idle callback function */
	glutKeyboardFunc(Keystroke);	//regster keyboard function
	initScene();

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 360);
	glutEnterGameMode();
	quadric = gluNewQuadric();
	glutCreateMenu(mainmenu);
	glutAddMenuEntry("QUIT", 1);
	glutAddMenuEntry("DAY VIEW", 2);
	glutAddMenuEntry("NIGHT VIEW", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// register all callbacks
	init();
	initWindow();
	printf("Right click on screen to choose night/day mood or quit \n");
	printf("press o | O | i | I to move ship  diagonaly \n");
	printf("press f | b | l | r to move ship \n");
	printf("Press c to change the direction boats\n");
	printf("Press P to pause and s to resume\n");
	glutMainLoop();
	return(0);
}