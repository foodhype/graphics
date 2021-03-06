/********************************************************************

  twowindows.cpp - based upon Angel's rotating cube program in appendix A9
  Written by H. Masterman to illustrate how GLUT can be used to generate
  multiple windows

  rev. 0 28 April 2003
  
  continually revised to demonstrate various effects - so no two versions may be
  exactly alike, unfortunately.  Portions of this code may be used in your programs,
  but I would prefer that you make your own version.

  ********************************************************************/

#include <stdlib.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <iostream>
using namespace std;


	GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
	{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

	
	GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
	{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};


int window1, window2;  // identifiers for windows

void polygon(int a, int b, int c , int d)
{

/* draw a polygon via list of vertices */

 	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube(void)
{

/* map vertices to faces */

	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);
}

static GLfloat theta1 = 0.0; //angle of rotation

void window1_display(void)  //display callback for window1
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
	glLoadIdentity();  // start with an identity in the MODELVIEW transformation
	gluLookAt(0.0,3.0,7.0, 0.0,0.0,0.0,0,1,0);  //multiply viewing component of transformation
	glRotatef(theta1, 0,1,0);

	colorcube(); // draw the color cube

	glFlush();
	glutSwapBuffers();
}
 
void window2_display(void)  //display callback for window2
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
	glLoadIdentity();  // start with an identity in the MODELVIEW transformation

	//rotate to give a top down view like you might want to do in a CAD program
	glRotatef(90, 1,0,0);

	glRotatef(theta1, 0,1,0);

	colorcube(); // draw the color cube

	glFlush();
	glutSwapBuffers();
}




void spinCube()
{

//idle callback angle is incremented by 1 degree each time it is called

	theta1 += .05; //rotation angle

	if( theta1 > 360.0 ) theta1 -= 360.0;
 
	glutSetWindow(window1);
	glutPostRedisplay();  // generate a display event for window1

	glutSetWindow(window2);
	glutPostRedisplay();  // generate a display event for window2
	//cout<<"the current window is: "<<glutGetWindow()<<endl;
}

void window1_mouse(int btn, int state, int x, int y)
{
cout<<"I am in window 1"<<endl;

}

void window2_mouse(int btn, int state, int x, int y)
{
cout<<"I am in window 2"<<endl;

}

void window1_keyboard(unsigned char key, int x,int y)
{
	cout<<"Character:  "<<key<<" typed in window 1"<<endl;
}


void window2_keyboard(unsigned char key, int x,int y)
{
	cout<<"Character:  "<<key<<" typed in window 2"<<endl;
}
void window1_reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0,1.0,30.0);  //window 1 has a perspective view
    glMatrixMode(GL_MODELVIEW);
	
}

void window2_reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-5,5,-5,5,-5,5);  //window 1 has a orthographic view
    glMatrixMode(GL_MODELVIEW);
	
}

int
main(int argc, char **argv)
{
    glutInit(&argc, argv);

/* need both double buffering and z buffer */

   
    glutInitWindowSize(500, 500);
	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	// create the first window
	window1 = glutCreateWindow("First Window - Perspective View");
    // register callbacks for first window, which is now current
	glutReshapeFunc(window1_reshape);
    glutDisplayFunc(window1_display);
	glutMouseFunc(window1_mouse);
	glutKeyboardFunc(window1_keyboard);

	//create the second  window
	window2 = glutCreateWindow("Second Window - Top/Down View");
	//define a window position for second window
	glutPositionWindow(520,30);
    // register callbacks for second window, which is now current
	glutReshapeFunc(window2_reshape);
    glutDisplayFunc(window2_display);
	glutMouseFunc(window2_mouse);  //note we share the mouse function
	glutKeyboardFunc(window2_keyboard);
	
	glutIdleFunc(spinCube);  //idle function is not associated with a window
	glutMainLoop();
}
