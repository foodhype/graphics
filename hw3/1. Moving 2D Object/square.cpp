//-------------------------------------------------------------------
// Modified By: Alejandro Montoya
//
// This is a simple OpenGL program that demonstrates the use of Euler transformation with OpenGL
//
//-------------------------------------------------------------------

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

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#define SPIN	1
#define STOP	2
#define QUIT	3

// Create an enuerated type for the special keys recognized by OpenGL.
// These include function keys and arrow keys.
enum special_key {
    left_arrow_key  = 100,
    up_arrow_key    = 101,
    right_arrow_key = 102,
    down_arrow_key  = 103
};

using std::cout;
using std::endl;

static GLfloat fSquareAngle = 0.0;
static GLfloat fSquareX = 0.0;
static GLfloat fSquareY = 0.0;
static GLfloat fSquareScale = 1.0;
static const double delta = 0.1;

#ifdef _WIN32
    static DWORD last_idle_time;
#else
    static struct timeval last_idle_time;
#endif

// A simple function for drawing a fixed triangle with a border.
// By default, the triangle is opaque.  If an alpha less than one
// is specified, the triangle will be semi-transparent.
void draw_square( double alpha = 1.0 ) {
    // Set the color that will be used to fill the triangle.
    glColor4f( 0.55, 0.55, 0.55, alpha );
    glBegin( GL_POLYGON );
        glVertex2f( 1, 1 );
        glVertex2f( -1, 1 );
        glVertex2f( -1, -1 );
        glVertex2f( 1, -1 );
    glEnd();
    
    // Set the color and width of the border around the triangle.
    glLineWidth( 5 ); // Make the triangle border fat.
    glColor4f( 1.0, 1.0, 1.0, alpha );
    glBegin( GL_LINE_LOOP );
        glVertex2f( 1, 1 );
        glVertex2f( -1, 1 );
        glVertex2f( -1, -1 );
        glVertex2f( 1, -1 );
    glEnd();
}

// This function is called by OpenGL whenever the display
// must be updated.  It manipulates OpenGL's "modeling"
// matrix in order to draw the untransformed coordinate axes
// followed by the transformed triangle.
void display( void ){
    // clear frame buffer.
    glClear( GL_COLOR_BUFFER_BIT );
    
    // Indicate that we will be modifying the modeling matrix, not
    // the viewing matrix.
    glMatrixMode( GL_MODELVIEW );
    
    // Initially, set the modeling matrix to the identity, so that
    // we can draw the coordinate axes.
    glLoadIdentity();
    glColor3f( 1, 0, 0 );
    glLineWidth( 3 );
    glBegin( GL_LINES );
        glVertex2f(  0,  2 );
        glVertex2f(  0, -2 );
        glVertex2f(  2,  0 );
        glVertex2f( -2,  0 );
    glEnd();
    
    // Now load the global modeling matrix M and draw the triangle.
    // Everything drawn after the "Load" command will be transformed
    // by the given matrix.
    
    glScalef(fSquareScale,fSquareScale,1);
    glTranslatef(fSquareX, fSquareY, 0);
    glRotatef(fSquareAngle, 0, 0, 1);
    
    draw_square( 0.70 );
    
    // Flush the commands sent to OpenGL and "swap" the buffers, so that
    // what was just drawn in the "back" buffer becomes visible.  This is
    // part of "double buffering".
    glFlush();
    glutSwapBuffers();
}

void AnimateScene(void) {
    float dt;
#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    // Animate the teapot by updating its angles
    fSquareAngle += dt * 30.0;
    // Save time_now for next time
    last_idle_time = time_now;
    // Force redraw
    glutPostRedisplay();
}

// Process key-press events.
void key_press( unsigned char key, int x, int y ) {
    switch( key ) {
        case 'r':
            // When the matrix is reset to the identity, indicate to OpenGL
            // that the image should be re-drawn.
            fSquareAngle = fSquareY = fSquareX = 0.0;
            fSquareScale = 1.0;
            glutPostRedisplay();
            break;
        case 'a': // rotate delta
            fSquareAngle+=5.0;
            glutPostRedisplay();
            break;
        case 's': //rotate -delta
            fSquareAngle-=5.0;
            glutPostRedisplay();
            break;
        case 'y': //scale 0.9
            fSquareScale-=delta;
            glutPostRedisplay();
            break;
        case 't':
            fSquareScale+=delta;
            glutPostRedisplay();
            break;
        case 'q': //exit
        case 'Q':
            exit(0);
    }
}

void menu (int item)
{
    switch(item)  {
        case  SPIN:
            glutIdleFunc(AnimateScene);
            break;
        case  STOP:
            glutIdleFunc(NULL);
            break;
        case  QUIT:
            exit(0);
            break;
    }
}

// Process "special" key-press events.  This includes the arrow keys, the
// "home" key, and the "PageUp" and "PageDown" keys.  Notice that the first
// argument is an "int" rather than an "unsighed char".
void special_key_press( int key, int x, int y ) {
    switch( key ) {
        case right_arrow_key: // transtale right
            fSquareX+=delta;
            glutPostRedisplay();
            break;
        case left_arrow_key: //translate left
            fSquareX-=delta;
            glutPostRedisplay();
            break;
        case up_arrow_key: //translate up
            fSquareY+=delta;
            glutPostRedisplay();
            break;
        case down_arrow_key: //translate down
            fSquareY-=delta;
            glutPostRedisplay();
            break;
    }
}

int main( int argc, char** argv ) {
    // Do some basic initialization and setup.  Here is
    // where we set the initial size of the graphics window,
    // its placement on the screen, the background color, etc.
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( 500, 500 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Demo: Moving triangle" );
    glClearColor( 0.20, 0.30, 0.50, 0.0 ); //clear window.
    
    // Initialize the projection to be orthographic such that
    // the coordinates of the points range from -2 to 2 in all
    // three coordinates.  The XY coordinates will correspond to
    // the screen.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -3.0, 3.0, -3.0, 3.0, -3.0, 3.0 );
    
    // Set the various callback functions, then let OpenGL take over.
    glutDisplayFunc( display );
    glutKeyboardFunc( key_press );
    glutSpecialFunc( special_key_press );
    
    glutCreateMenu(menu);
    glutAddMenuEntry("Spin the Square", SPIN);
    glutAddMenuEntry("Stop Spinning", STOP);
    glutAddMenuEntry("Quit the Program", QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    // Tell OpenGL that we wish to use alpha blending in order to make
    // some objects semi-transparent.  Also ask OpenGL to "smooth" lines
    // so that they do not look jaggy.
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glEnable( GL_LINE_SMOOTH );
    
#ifdef _WIN32
    last_idle_time = GetTickCount();
#else
    gettimeofday (&last_idle_time, NULL);
#endif
    
    // Print out instructions...
    cout << "The arrow keys translate the triangle.\n"
    << "The 'a' and 's' keys rotate the triangle.\n"
    << "The 'r' key re-centers the triangle.\n"
    << "The 't' and 'y' keys scalate the triangle.\n"
    << "The Q key exits the program.\n"
    << endl;
    
    // Give OpenGL control, which is never returned to this program.
    glutMainLoop();
    return 0;
}
