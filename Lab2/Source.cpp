/* lab2.cpp
Draws a circle in the center of the screen that bounces off of the screen boundaries and changes color.
*/

/*	I used the new_hello.c file as the starting template for this assignment. The randomDouble() method, as well
	as some initialization code comes from new_hello.c. The drawCircle() method was adapted from http://slabode.exofire.net/circle_draw.shtml.
	The circle does not maintain size or aspect ratio when the window is resized. I eventually did find a way to do it online,
	but I did not understand what the code was doing and I did not want to blantantly copy the code. I could not get gluPerspective()
	to work properly as it did in the tutorials (because I am using 2D shapes rather than 3D, perhaps?), so I ended up just using glViewport(). 
	This program works by drawing the face to screen, clearing it, and redrawing it in a different location so as to simulate movement. */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>

float xPos = 0.5;
float yPos = 0.5;

float vX = 0.02;
float vY = 0.03;

double red = 1.0;
double green = 1.0;
double blue = 1.0;

float radius = 0.1;

float xBoundary = 1.0 - radius;
float yBoundary = 1.0 - radius;

void init(void)
{
	/* select clearing color as white */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* initialize viewing values  */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
}

double randomDouble()
/* Return a random double, drawn with uniform probability from [0,1]. */
{
	return rand() / (double)RAND_MAX;
}

void drawCircle(float x, float y, float radius)
{
	// This function will draw 30 line segments to the screen. This is adapted from the first function on http://slabode.exofire.net/circle_draw.shtml

	glBegin(GL_POLYGON);									// GL_POLYGON to fill the circle with a color.
	for (int i = 0; i < 30; i++)
	{
		float angle = 2.0 * (atan(1.0)*4) * i / 30.0;		// get the angle of the current line segment. atan(1)*4 = PI

		float x0 = radius * cos(angle);						// get the X
		float y0 = radius * sin(angle);						// get the Y

		glVertex2f(x0 + x, y0 + y);							// draw the line segment to screen.

	}
	glEnd();
}

void drawMouth() {
	// The mouth is drawn using a trapezoidal-like polygon.

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(xPos - 0.06, yPos - 0.04);
	glVertex2f(xPos + 0.06, yPos - 0.04);
	glVertex2f(xPos + 0.03, yPos - 0.08);
	glVertex2f(xPos - 0.03, yPos - 0.08);
	glEnd();
}

void changeColor() {
	// Get 3 random variables for RGB.
	red = randomDouble();
	green = randomDouble();
	blue = randomDouble();
}

void drawFace() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(red, green, blue);

	drawCircle(xPos, yPos, radius);					// Head

	glColor3f(0.00, 0.19, 0.56);
	drawCircle(xPos - 0.04, yPos + 0.04, 0.03);		// Left Eye
	drawCircle(xPos + 0.04, yPos + 0.04, 0.03);		// Right Eye
	drawMouth();									// Mouth

	xPos += vX;		// 'Move' head on the x axis
	yPos += vY;		// 'Move' head on the y axis

	if (xPos >= xBoundary || xPos <= 0.0 + radius) {		// Check for X boundary collision
		vX *= -1;
		changeColor();
	}

	if (yPos >= yBoundary || yPos <= 0.0 + radius) {		// Check for Y boundary collision
		vY *= -1;
		changeColor();
	}
}

void display(void)
{
	drawFace();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 113 || key == 81) {
		exit(0);						// terminate is q or Q is pressed.
	}
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		exit(0);						// terminate if right mouse button is clicked.
	}
}

int main(int argc, char** argv)
/*
* Declare initial window size, position, and
* display mode (single buffer and RGBA).
* Open window with "hello" * in its title bar.
* Call initialization routines.
* Register callback function to display graphics.
* Enter main loop and process events.
*/
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 500)/2, (glutGet(GLUT_SCREEN_HEIGHT) - 500)/2);	// center window on screen
	glutCreateWindow("Lab #2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;   /* ANSI C requires main to return int. */
}