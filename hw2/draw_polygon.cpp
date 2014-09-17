// A trivial OpenGL program that draws a triangle, and prints
// out screen coordinates in response to various events, such as
// mouse clicks and cursor motion.
//-------------------------------------------------------------------

#include <iostream>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__
#include <vector>

using std::cout;
using std::endl;
std::vector<std::pair<int, int> > points;
std::vector<std::vector<std::pair<int, int> > > polygons;

// This function is called by OpenGL whenever the display
// must be updated.  ALL drawing operations should be
// performed via this function.  OpenGL will call this
// function at various times, such as at startup, and
// whenever glutPostRedisplay() is called.
void display( void ) {
    glClear( GL_COLOR_BUFFER_BIT ); // clear frame buffer
    glColor3f( 0.0, 1.0, 0.0 );     // (red,green,blue)
    for (std::vector<std::vector<std::pair<int, int> > >::iterator polygon = polygons.begin();
            polygon != polygons.end();
            ++polygon) {
        glBegin( GL_POLYGON );
        for (std::vector<std::pair<int, int> >::iterator coord = polygon->begin();
                coord != polygon->end();
                ++coord) {
            glVertex3f((double) coord->first, (double) coord->second, 0.0);
        }
        glEnd();
    }
    glFlush();
}

// This is the function that we will tell OpenGL to call
// whenever the cursor is moved WHILE a button is held down.
void mouse_motion( int x, int y ) {
    cout << "Cursor moved (with button down) to " << x << ", " << y << endl;
}

// This is the function that we will tell OpenGL to call
// whenever the cursor is moved no button held down.
void mouse_passive_motion( int x, int y ) {
    cout << "Cursor moved to " << x << ", " << y << endl;
}

// This is the function that we will tell OpenGL to call
// whenever a mouse button is pressed or released.
void mouse_button( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        cout << "Left button down at " << x << ", " << y << endl;
        points.push_back(std::make_pair<int, int>(x, y));
    }
    else if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    {
        cout << "Left button up at "  << x << ", " << y << endl;
    }
    else if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        cout << "Right button down ar " << x << ", " << y << endl;
    }
    else if( button == GLUT_RIGHT_BUTTON && state == GLUT_UP )
    {
        cout << "Right button up at " << x << ", " << y << endl;
    }
}

// This is the function that we will tell OpenGL to call
// whenever a key on the keyboard is pressed.
void key_press( unsigned char key, int x, int y )
{
    // inefficient but reasonable for testing purposes.
    std::vector<std::pair<int, int> > temp(points);

    switch( key )
    {
        case ' ':
            polygons.push_back(temp);
            glutPostRedisplay();
            points.clear();
            break;
        case 'q':
            break;
        case 'Q':
            exit(0);
            break;
    }
}

int main( int argc, char** argv )
{
    // Do some basic initialization and setup.  Here is
    // where we set the initial size of the graphics window,
    // its placement on the screen, the background color, etc.
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize( 400, 400 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "My first OpenGL program" );
    glClearColor( 0.20, 0.30, 0.50, 0.0 ); // clear window.
    
    // Initialize the projection to be orthographic such that
    // the coordinates of the points range from -2 to 2 in all
    // three coordinates.  The XY coordinates will correspond to
    // the screen.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glOrtho( -2.0, 2.0, -2.0, 2.0, -2.0, 2.0 );
    glOrtho(0.0f, 400.0, 400.0, 0.0f, 0.0f, 2.0f);
    
    // Set the various callback functions, then let OpenGL take over.
    glutDisplayFunc( display );
    glutMotionFunc( mouse_motion );
    glutPassiveMotionFunc( mouse_passive_motion );
    glutMouseFunc( mouse_button );
    glutKeyboardFunc( key_press );
    
    // Give OpenGL control, which is never returned to this program.
    glutMainLoop();
    return 0;
}
