//http://www.opengl.org/archives/resources/faq/technical/lookat.cpp

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

int windowWidth;

// Rotate the scene about the Y axis
GLfloat yrot;

// Location of the target and camera objects, respectively
float target[3], camera[3];


#define DELTA 0.25f
static void key (unsigned char k, int x, int y) {
    switch (k) {
        case 'q': target[0] -= DELTA; break;
        case 'Q': target[0] += DELTA; break;
        case 'w': target[1] -= DELTA; break;
        case 'W': target[1] += DELTA; break;
        case 'e': target[2] -= DELTA; break;
        case 'E': target[2] += DELTA; break;
        case 'a': camera[0] -= DELTA; break;
        case 'A': camera[0] += DELTA; break;
        case 's': camera[1] -= DELTA; break;
        case 'S': camera[1] += DELTA; break;
        case 'd': camera[2] -= DELTA; break;
        case 'D': camera[2] += DELTA; break;
            break;
        default:
            exit(0);
    }
    glutPostRedisplay ();
}


// Calculate the cross product and return it
static void cross (float dst[3], float srcA[3], float srcB[3])
{
    dst[0] = srcA[1]*srcB[2] - srcA[2]*srcB[1];
    dst[1] = srcA[2]*srcB[0] - srcA[0]*srcB[2];
    dst[2] = srcA[0]*srcB[1] - srcA[1]*srcB[0];
}

// Normalize the input vector
static void normalize (float vec[3])
{
    const float squaredLen = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    const float invLen = 1.f / (float) sqrt (squaredLen);
    
    vec[0] *= invLen;
    vec[1] *= invLen;
    vec[2] *= invLen;
}

// Scale the given vector
static void scale (float v[3], float s)
{
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
}

/*
 multLookAt -- Create a matrix to make an object, such as
 a camera, "look at" another object or location, from
 a specified position.
 
 Parameters:
 eye[x|y|z] Desired location of the camera object
 at[x|y|z]  Location for the camera to look at
 up[x|y|z]  Up vector of the camera
 
 Algorithm:
 The desired transformation is obtained with this 4x4 matrix:
 |  [xaxis] 0  |
 |    [up]  0  |
 |   [-at]  0  |
 |   [eye]  1  |
 Where 'xaxis', 'up' and 'at' are the new X, Y, and Z axes of
 the transforned object, respectively, and 'eye' is the input
 new location of the transformed object.
 
 Assumptions:
 The camera geometry is defined to be facing
 the negative Z axis.
 
 Usage:
 multLookAt creates a matrix and multiplies it onto the
 current matrix stack. Typical usage would be as follows:
 
 glMatrixMode (GL_MODELVIEW);
 // Define the usual view transformation here using
 //   gluLookAt or whatever.
 glPushMatrix();
 multLookAt (orig[0], orig[1], orig[2],
 at[0], at[1], at[2],
 up[0], up[1], up[2]);
 // Define "camera" object geometry here
 glPopMatrix();
 
 Warning: Results become undefined as (at-eye) approaches
 coincidence with (up).
 */
static void multLookAt (float eyex, float eyey, float eyez,
                        float atx, float aty, float atz,
                        float upx, float upy, float upz)
{
    float m[16];
    float *xaxis = &m[0],
    *up = &m[4],
    *at = &m[8];
    
    // Compute our new look at vector, which will be
    //   the new negative Z axis of our transformed object.
    at[0] = atx-eyex; at[1] = aty-eyey; at[2] = atz-eyez;
    normalize (at);
    
    // Make a useable copy of the current up vector.
    up[0] = upx; up[1] = upy; up[2] = upz;
    
    // Cross product of the new look at vector and the current
    //   up vector will produce a vector which is the new
    //   positive X axis of our transformed object.
    cross (xaxis, at, up);
    normalize (xaxis);
    
    // Calculate the new up vector, which will be the
    //   positive Y axis of our transformed object. Note
    //   that it will lie in the same plane as the new
    //   look at vector and the old up vector.
    cross (up, xaxis, at);
    
    // Account for the fact that the geometry will be defined to
    //   point along the negative Z axis.
    scale (at, -1.f);
    
    // Fill out the rest of the 4x4 matrix
    m[3] = 0.f;     // xaxis is m[0..2]
    m[7] = 0.f;     // up is m[4..6]
    m[11] = 0.f;    // -at is m[8..10]
    m[12] = eyex; m[13] = eyey; m[14] = eyez;
    m[15] = 1.f;
    
    // Multiply onto current matrix stack.
    glMultMatrixf (m);
}


static void display( void )
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    // View transform, place 'eye' at (0,1,5), and allow the scene
    //   to rotate around its local Y axis in front of the eye.
    glTranslatef (0., -1., -5.);
    glRotatef (yrot, 0., 1., 0.);
    
    // Draw the target
    glColor3f (.7, .4, 1.);
    glPushMatrix ();
        glTranslatef (target[0], target[1], target[2]);
        glutWireSphere(0.5,20,16);
    glPopMatrix ();
    
    glColor3f (1., .6, .4);
    glPushMatrix ();
        // Create transform so our next set of geometry will
        //   "look at" the target.
        multLookAt (camera[0], camera[1], camera[2],
                    target[0], target[1], target[2],
                    0., 1., 0.);
        // Now draw our "camera" which is just an orange cone
        //   pointing at the target and a white cone indicating
        //   the camera's local "up" direction.
        glRotatef (180., 1., 0., 0.);
        glutWireCone(0.5, 1.0, 20, 16);
    glPopMatrix ();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth=w;
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (50., (float)w/(float)h, 1., 20.);
}

static void mouse (int x, int y) {
    yrot = (float)x*360.f/(float)windowWidth - 180.f;
    glutPostRedisplay ();
}

static void init () {
    target[0] = target[1] = target[2] = 1.25f;
    camera[0] = camera[1] = camera[2] = 0.f;
    yrot = 0.f;
    
    glEnable (GL_DEPTH_TEST);
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutMotionFunc (mouse);
    glutKeyboardFunc (key);
}

int main(int argc, char** argv)
{
    glutInit (&argc,argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (windowWidth=500,500);
    glutInitWindowPosition (100,100);
    glutCreateWindow ("Looking at an object");
    
    init ();
    
    printf ("Left mouse button rotates the scene.\n");
    printf ("Move the target object:\n");
    printf ("\tq/Q\talong the X axis;\n");
    printf ("\tw/W\talong the Y axis;\n");
    printf ("\te/E\talong the Z axis.\n");
    printf ("Move the camera object:\n");
    printf ("\ta/A\talong the X axis;\n");
    printf ("\ts/S\talong the Y axis;\n");
    printf ("\td/D\talong the Z axis.\n");
    printf ("Any other key exits.\n");
    
    glutMainLoop ();
}