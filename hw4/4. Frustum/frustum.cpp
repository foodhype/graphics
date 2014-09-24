// This is a simple OpenGL program that demonstrates how to set up a
// viewing frustum in 3D using the functions glFrustum and gluLookAt.
// The effects of changing the eye position are demonstrated by
// mapping various keys to shifts in the eye coordinates.  The gaze
// direction is always toward the origin.

// Include standard system header files.
#include <iostream>
#include <cmath>

// Include the GLUT header, which will include both gl.h and glu.h.
// You may need to modify this, depending on where this header is located
// on your machine, and how you have the programming environment configured.
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include "Vec3.h"

using std::cout;
using std::endl;

int WindowSize = 600;
Vec3 eye ( 3, 1, 3 );
Vec3 cent( 0, 0, 0 );
Vec3 up  ( 0, 0, 1 );
double znear = 3.0;
double zfar  = 10.0;
double hw    = 1.0;  // half-width of viewing window.

// Draw a canonical square centered at the origin in the XY plane.
void draw_square() {
    glColor3f( 0.5, 0.7, 0.7 );
    glBegin( GL_POLYGON );
        glVertex3f( -1, -1,  0 );
        glVertex3f( -1,  1,  0 );
        glVertex3f(  1,  1,  0 );
        glVertex3f(  1, -1,  0 );
    glEnd();
}

void display( void ) {
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
              
    
    glColor3f( 1.0, 1.0, 1.0 );
    
    // Draw a scaled and translated sphere.
    glPushMatrix();
        glTranslatef(0, -0.35, 0.25);// This post-multiplies current MODELVIEW matrix.
        glScalef(0.25,0.25,0.25);// This post-multiplies current MODELVIEW matrix again.
        glutWireSphere(1.0,20,16);
    glPopMatrix();  // Go back to original "LookAt" matrix.
    
    // Draw another scaled and translated sphere.
    glPushMatrix();
        glTranslatef( 0, 0.35, 0.35 );  // This post-multiplies current MODELVIEW matrix.
        glScalef( 0.35, 0.35, 0.35);           // This post-multiplies current MODELVIEW matrix again.
        glutWireSphere(1.0,20,16);
    glPopMatrix(); // Go back to original "LookAt" matrix.
    
    draw_square();
    glFlush();
    glutSwapBuffers();
}

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
        case 'q':
        case 'Q':
            exit(0);
    }
}

int main( int argc, char** argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( WindowSize, WindowSize );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Example of glFrustum" );
    glClearColor( 0.20, 0.30, 0.50, 0.0 ); // clear window.
    
    // Set the various callback functions, then let OpenGL take over.
    glutDisplayFunc( display );
    glutKeyboardFunc( key_press );
    
    glEnable( GL_DEPTH_TEST ); // Turn on depth buffering.
    // Give OpenGL control, which is never returned to this program.
    glutMainLoop();
    return 0;
}
