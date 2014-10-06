/*  Light1.c   This programs draws a simple Solar system that animates
 constantly
 */

#include <math.h>
#include <stdlib.h>
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

#ifdef _WIN32
static DWORD last_idle_time;
#else
static struct timeval last_idle_time;
#endif

static double x; // This value will be remembered each time display is called

static void Init() {                                 // Initialization routine
    glClearColor(0.3, 0.3, 0.3, 0.0);                  // Set background to gray
    glEnable(GL_DEPTH_TEST); 		       // Enable depth buffering
}

static void Reshape(int width, int height) {         //  Determines Projection type
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //set up a perspective projection matrix
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.00, 30.0);
    // Perspective Parms( Angle, Aspect Ratio, Front Clipping, Back Clipping)
    
    //gluLookAt(0.0, 2.5, 10.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);

    gluLookAt(0.0, 2.5, 5.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
// LookAt Parms (Eye_X, Eye_Y, Eye_Z, View_X, View_Y, View_Z, Up_X, Up_Y, UpZ)
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    // Animate the scene by Increment 2 degrees each call
    x+=2;
    // Save time_now for next time
    last_idle_time = time_now;
    // Force redraw
    glutPostRedisplay();
}




void wireCube(GLfloat aSize) {
    static GLfloat vertices[] = {
        // top face
        -1.f, 1.f, 1.f,   // end edge 0
        1.f, 1.f, 1.f,    // start edge 1
        1.f, 1.f, 1.f,    // end edge 1
        1.f, 1.f, -1.f,   // start edge 2
        1.f, 1.f, -1.f,   // end edge 2
        -1.f, 1.f, -1.f,  // start edge 3
        -1.f, 1.f, -1.f,  // end edge 3
        -1.f, 1.f, 1.f,   // start edge 0

        // bottom face
        -1.f, -1.f, 1.f,  // end edge 0
        1.f, -1.f, 1.f,   // start edge 1
        1.f, -1.f, 1.f,   // end edge 1
        1.f, -1.f, -1.f,  // start edge 2
        1.f, -1.f, -1.f,  // end edge 2
        -1.f, -1.f, -1.f, // start edge 3
        -1.f, -1.f, -1.f, // end edge 3
        -1.f, -1.f, 1.f,  // start edge 0

        // side edges
        -1.f, -1.f, 1.f,  // bottom edge 0
        -1.f, 1.f, 1.f,   // top edge 0
        1.f, -1.f, 1.f,   // bottom edge 1
        1.f, 1.f, 1.f,    // top edge 1
        1.f, -1.f, -1.f,  // bottom edge 2
        1.f, 1.f, -1.f,   // top edge 2
        -1.f, -1.f, -1.f, // bottom edge 3
        -1.f, 1.f, -1.f   // top edge 3
    };

    static GLbyte normal_vectors[] = {
        // top face
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,

        // bottom face
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,

        // side edges
        0, 0, 1,
        0, 0, 1,
        1, 0, 0,
        1, 0, 0,
        0, 0, -1,
        0, 0, -1,
        -1, 0, 0,
        -1, 0, 0
    };

    glMatrixMode(GL_MODELVIEW);
    // We want to draw scaled object then restore its values to default.
    // We can do this in-place, so it is more efficient.
    glPushMatrix();

    // Set array pointers
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_BYTE, 0, normal_vectors);

    // Scale to size.
    glScalef(aSize, aSize, aSize);
    // Draw lines for 24 vertices.
    glDrawArrays(GL_LINES, 0, 24);
    // Restore defaults.
    glPopMatrix();
}


static void display() {

    // Minecraft planets

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear Color and Depth
    glPushMatrix(); // Begin drawing image
        glRotatef(x, 0.0, 1.0, 0.0); // rotate
        glColor3f(1.0, 1.0, 0.0); // Set drawing color to yellow
        wireCube(0.5); // Draw sun
        glPushMatrix(); // Draw Mercury
            glTranslatef(1.0, 0.0, -0.8); // Move away from origin
            glColor3f(1.0, 0.0, 0.0); // Set color to red
            glRotatef(x, 0.0, 0.0001, 0.0);
            wireCube(0.15);
        glPopMatrix(); // Return to first refernce
        glPushMatrix();	// Venus
            glTranslatef(0.5, 0.2, -1.4); // Move away from origin
            glColor3f(1.0, 0.2, 0.0); // Set color to orange-red
            glRotatef(x, 0.0, 0.0001, 0.0);
            wireCube(0.23);
            glPushMatrix();
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -4.0);
                glColor3f(0.8, 0.0, 5.0);
                wireCube(0.2);
            glPopMatrix();			  // Return to former reference
            glPushMatrix();
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -3.5);
                glColor3f(1.0, 0.0, 8.0);
                wireCube(0.2);
            glPopMatrix(); // Return to first refernce
        glPopMatrix(); // Return to first refernce
        glPushMatrix();
            glTranslatef(0.5, -0.5, 1.8); // Move away from origin
            glColor3f(0.0, 0.0, 1.0); // Set color to blue
            glRotatef(x, 0.0, 0.0001, 0.0);
            wireCube(0.3); // Draw Earth
            glPushMatrix();
                glRotatef(x, 0.0, 1.0, 0.0); // Rotate
                glTranslatef(0.3, 0.0, -0.5); // Move reference point
                glColor3f(1.0, 1.0, 1.0); // set color to gray
                wireCube(0.1);
            glPopMatrix(); // Return to former reference
        glPopMatrix(); // Return to first refernce
        glPushMatrix();	// Draw fake planet
            glTranslatef(2.2, -0.5, 1.8); // Move away from origin
            glColor3f(1.0, 0.0, 1.0); // Set color to magenta
            glRotatef(x, 0.0, 0.0001, 0.0);
            wireCube(0.3);
            glPushMatrix();
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -5.0);
                glColor3f(0.5, 0.8, 0.5);
                wireCube(0.12);
                glPushMatrix();
                    glRotatef(x, 0.0, 1.0, 0.0);
                    glTranslatef(0.3, 0.0, -1.0);
                    glColor3f(1.0, 0.0, 8.0);
                    wireCube(0.2);
                glPopMatrix();			  // Return to former reference
            glPopMatrix();			  // Return to former reference
        glPopMatrix();			

/*        glPushMatrix();			  // Draw orbiting sphere
            glTranslatef(0.5, 0.0, -1.0);	  // Move away from origin
            glColor3f(1.0, 0.0, 0.0);             // Set color to red
            wireCube(0.3);
            glPushMatrix();			     // Draw glutWireTeapot orbiting around the sphere
                glRotatef(x, 0.0, 1.0, 0.0);	     // Rotate
                glTranslatef(0.3, 0.0,-0.5);	     // Move reference point
                glColor3f(0.0, 0.0, 1.0);		     // Color Blue
                wireCube(0.2);
            glPopMatrix();			  // Return to former reference
        glPopMatrix();			// Return to first refernce


        glPushMatrix();			  // Draw orbiting cone
            glTranslatef(-0.5, 0.0, 1.0);	  // Move reference poit
            glColor3f(0.0, 1.0, 0.0);		  // Set color to green
            wireCube(0.3);
        glPopMatrix();			// Return to first reference
        glPushMatrix();
            glTranslatef(0.5, 0.0, -1.0);   // Move away from origin
            glColor3f(1.0, 1.0, 0.0);
            wireCube(0.5);
        glPopMatrix(); */
    glPopMatrix();		      // Return to base reference
    glFlush();			      // Flush drawing routines
    glutSwapBuffers();		      // Swap buffers
}

int main(int argc, char** argv) {	// Run program with possible parameters
    // "glut" Initialization Functions rather than "aux" versions
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Initialize modes
    glutInitWindowSize( 550, 550);
    glutInitWindowPosition( 50, 50);
    glutCreateWindow( argv[0]); // Note that arg[0] is name of program (Window name)
    Init();
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutIdleFunc(AnimateScene);
    glutMainLoop();
    return 0;
}
