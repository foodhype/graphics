/* ======================================================================
 *
 * draw.c - Draw a filled polygon.
 *
 * Written by: George Wolberg, 2012
 * ======================================================================
 */

#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#import "glm.h"
#import "Angel.h"
#include "Vec3.h"

using std::cout;
using std::endl;

int WindowSize = 600;
bool mode = 0;
Vec3 eye ( 3, 1, 3 );
Vec3 cent( 0, 0, 0 );
Vec3 up  ( 0, 1, 0 );
double znear = 3.0;
double zfar  = 10.0;
double hw    = 1.0;  // half-width of viewing window.


GLuint ProjLoc;
GLMmodel *objmodel_ptr;

// This is the function that we will tell OpenGL to call
// whenever a key on the keyboard is pressed.
void key_press( unsigned char key, int x, int y ) {
    switch( key ) {
        case 'w':  // Move eye up.  THIS IS A VERY CRUDE WAY TO DO IT!
            eye += 0.1 * Unit( ( -eye ^ up ) ^ ( -eye ) );
            glutPostRedisplay();
            break;
        case 's':  // Move eye down.  THIS IS A VERY CRUDE WAY TO DO IT!
            eye -= 0.1 * Unit( ( -eye ^ up ) ^ ( -eye ) );
            glutPostRedisplay();
            break;
        case 'a':  // Move eye to the left.  THIS IS A VERY CRUDE WAY TO DO IT!
            eye -= 0.1 * Unit( -eye ^ up );
            glutPostRedisplay();
            break;
        case 'd':  // Move the eye to the right.  THIS IS A VERY CRUDE WAY TO DO IT!
            eye += 0.1 * Unit( -eye ^ up );
            glutPostRedisplay();
            break;
        case 'z':  // Zoom in.
            hw -= 0.05;
            if( hw < 0.2 ) hw = 0.2;
            glutPostRedisplay();
            break;
        case 'x':  // Zoom out.
            hw += 0.05;
            glutPostRedisplay();
            break;
        case 'm':
            mode = !mode;
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * init:
 *
 * Initialize state variables.
 */
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);	/* set background color */
	glColor3f   (1.0, 1.0, 1.0);		/* set foreground color */
    
    if (!objmodel_ptr) {
        objmodel_ptr = glmReadOBJ("bunny.obj");
        if (!objmodel_ptr) {
            return;
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * display:
 *
 * Display callback function.
 */
void display(void) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear frame buffer
    
    // Set up the viewing frustum.  Zooming will be performed here by changing the size
    // of the XY extent of the frustum.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -hw, hw, -hw, hw, znear, zfar );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    gluLookAt(eye .x, eye .y, eye .z,  // Position of the eye.
              cent.x, cent.y, cent.z,  // The "look at" point, or "center".
              up  .x, up  .y, up  .z);   // The "up vector".
    
    glColor3f   (1.0, 1.0, 1.0);
    
    //select a polygon rasterization mode
    if (mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    
    
    
    glmDraw(objmodel_ptr, GLM_NONE);
   	glFlush();				/* draw to (off-screen) buffer*/
    	glutSwapBuffers();			/* swap buffer to see drawing */
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * main:
 *
 * Initialize window info, state variables, and callback functions.
 * Enter main loop and process events.
 */
int main(int argc, char** argv) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( WindowSize, WindowSize );
	glutInitWindowPosition(100, 100);	/* initialize window position */
	glutCreateWindow(argv[0]);		/* set titlebar name; create  */
	init();					/* initialize state variables */
    
    glutDisplayFunc( display );
    glutKeyboardFunc( key_press );
    
	glutMainLoop();				/* enter infinite event loop  */
	return 0;				/* ANSI C requires int main() */
}
