/* =============================================================================

orientation.cpp

Program to demonstrate the use of quaternions in the editing and interpolation
of orientations.

Original: H. Masterman
Modified by: Alejandro Montoya
===============================================================================*/

#define GLUT_BUILDING_LIB
#define PI 3.14159

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <iostream>
#include <stdlib.h>
#include "quaternion.h"

using std::cout;
using std::endl;

float rotate_x = 0.0;
float rotate_y = 0.0;
float rotate_z = 0.0;
float m[4][4];
float *p;
float theta;
axis a;
quaternion q;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(-2.0,0,0);

	glRotatef(rotate_z, 0,0,1);  //Euler rotations
	glRotatef(rotate_y, 0,1,0);
	glRotatef(rotate_x, 1,0,0);
    glutWireTeapot(1.0);

	// now let's get the MODELVIEW MATRIX
	p=&m[0][0];
	glGetFloatv(GL_MODELVIEW_MATRIX, p);
	
	// print out matrix
	for (int i = 0; i<4;i++)
		cout  <<   m[i][0]<< "       " <<m[i][1] << "       " << m[i][2] <<"       "<<  m[i][3] << endl;

	q = Matrix_To_Quaternion(m);  // convert the matrix to a quaternion
	Quaternion_To_Axis_Angle(q, a, &theta);  // convert the quaternion to an axis/angle representation

	cout << "Angle is: "<<theta<<endl;
	cout << "Axis is: "<< a[0] <<"     "<<a[1]<<"     "<<a[2]<<endl;

	// now let's draw the teapot again with the single rotation
	glLoadIdentity();
	glTranslatef(2.0, 0,0);
	glRotatef(theta, a[0], a[1], a[2]);
	glutWireTeapot(1.0);
	
	glFlush();
	glutSwapBuffers();
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w, 4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-4.0 * (GLfloat) w / (GLfloat) h, 4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void myinit() {
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    glClearColor (0.0, 0.0, 0.0, 1.0);
}

void key_press( unsigned char key, int x, int y ) {
    switch( key ) {
        case 'w': //decrease rotate_y
            rotate_y -= 5.0;
            glutPostRedisplay();
            break;
        case 's':
            rotate_y += 5.0; //increase rotate_y
            glutPostRedisplay();
            break;
        case 'a': //increase rotate_z
            rotate_z += 5.0;
            glutPostRedisplay();
            break;
        case 'd': //decrease rotate_z
            rotate_z -= 5.0;
            glutPostRedisplay();
            break;
        case 'q': //increase rotate_x
            rotate_x += 5.0;
            glutPostRedisplay();
            break;
        case 'e': //decrease rotate_x
            rotate_x -= 5.0;
            glutPostRedisplay();
            break;
        case 'c': //exit
        case 'C':
            exit(0);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    
	glutCreateWindow("Edit Orientation");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc( key_press );
    glutMainLoop();
	return 1;
}
