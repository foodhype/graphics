//http://www.prinmath.com/csci5229/F06/programs/ex8.c

/*
 *  Example 8: Projections
 *  'm' to switch modes (projections)
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 *  +/- changes field of view of rperspective
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__  // include Mac OS X verions of headers

#  include <OpenGL/OpenGL.h>

#  include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#  include <GL/glew.h>


#include <GL/glext.h>


#  include <GL/freeglut.h>

#  include <GL/freeglut_ext.h>

#endif  // __APPLE__

PFNGLWINDOWPOS2IPROC glWindowPos2i;

int axes[4] = {0, 0, 90, 0};       //  Display axes
int mode[4] = {1, 0, 0, 0};       //  Projection mode
int th[4] = {0, 0, 90, 30};         //  Azimuth of view anglint
int ph[4] = {0, 0, 0, 60};         //  Elevation of view angle
int fov[4] = {55, 55, 55, 55};       //  Field of view (for perspective)
double asp = 1.0;     //  Aspect ratio
double dim[4] = {4.0, 4.0, 10.0, 2.5};   //  Size of world

int main_window, window1, window2, window3, window4;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project(int idx)
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (mode[idx])
        gluPerspective(fov[idx], asp, 0.1, 4 * dim[idx]);
    //  Orthogonal projection
    else
        glOrtho(-asp * dim[idx],
                +asp * dim[idx],
                -dim[idx],
                +dim[idx],
                -dim[idx],
                +dim[idx]);

    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //  Cube
    glBegin(GL_QUADS);
    //  Front
    glColor3f(1,0,0);
    glVertex3f(-1.0,-1.0, 1.0);
    glVertex3f(+1.0,-1.0, 1.0);
    glVertex3f(+1.0,+1.0, 1.0);
    glVertex3f(-1.0,+1.0, 1.0);
    //  Back
    glColor3f(0,0,1);
    glVertex3f(+1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,+1.0,-1.0);
    glVertex3f(+1.0,+1.0,-1.0);
    //  Right
    glColor3f(1,1,0);
    glVertex3f(+1.0,-1.0,+1.0);
    glVertex3f(+1.0,-1.0,-1.0);
    glVertex3f(+1.0,+1.0,-1.0);
    glVertex3f(+1.0,+1.0,+1.0);
    //  Left
    glColor3f(0,1,0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,+1.0);
    glVertex3f(-1.0,+1.0,+1.0);
    glVertex3f(-1.0,+1.0,-1.0);
    //  Top
    glColor3f(0,1,1);
    glVertex3f(-1.0,+1.0,+1.0);
    glVertex3f(+1.0,+1.0,+1.0);
    glVertex3f(+1.0,+1.0,-1.0);
    glVertex3f(-1.0,+1.0,-1.0);
    //  Bottom
    glColor3f(1,0,1);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(+1.0,-1.0,-1.0);
    glVertex3f(+1.0,-1.0,+1.0);
    glVertex3f(-1.0,-1.0,+1.0);
    //  End
    glEnd();
    //  Undo transofrmations
    glPopMatrix();
}

void displayHandler(int idx) {
    int i,j,k;
    const double len = 1.5;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (mode[idx])
    {
        double Ex = -2 * dim[idx] * Sin(th[idx]) * Cos(ph[idx]);
        double Ey = +2 * dim[idx] * Sin(ph[idx]);
        double Ez = +2 * dim[idx] * Cos(th[idx]) * Cos(ph[idx]);
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, 1, 0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph[idx], 1, 0, 0);
        glRotatef(th[idx], 0, 1, 0);
    }
    //  Draw cubes
    for (i=-1; i<=1; i++)
        for (j=-1; j<=1; j++)
            for (k=-1; k<=1; k++)
                cube(i, j, k, 0.3, 0.3, 0.3, 0);
    
    //  Draw axes
    glColor3f(1,1,1);
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(len,0.0,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,len,0.0);
        glVertex3d(0.0,0.0,0.0);
        glVertex3d(0.0,0.0,len);
        glEnd();
        //  Label axes
        glRasterPos3d(len,0.0,0.0);
        Print("X");
        glRasterPos3d(0.0,len,0.0);
        Print("Y");
        glRasterPos3d(0.0,0.0,len);
        Print("Z");
    }


    //  Display parameters
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",
            th[idx],
            ph[idx],
            dim[idx],
            fov[idx],
            mode[idx] ? "Perpective" : "Orthogonal");

    //  Render the scene and make it visible
    glFlush();
    glutSwapBuffers();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display1()
{
    displayHandler(0);    
}

void display2()
{
    displayHandler(1);
}

void display3()
{
    displayHandler(2);    
}

void display4()
{
    displayHandler(3);
}

void main_window_display()
{
}

void idleLoop() {
    glutSetWindow(window1);
    //display1();
    glutPostRedisplay();
    glutSetWindow(window2);
    //display2();
    glutPostRedisplay();
    glutSetWindow(window3);
    //display3();
    glutPostRedisplay();
    glutSetWindow(window4);
    //display4();
    glutPostRedisplay();
}

void mouse_func(int btn, int state, int x, int y) {
    exit(0);
}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y, int idx)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th[idx] += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th[idx] -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph[idx] += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph[idx] -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
        dim[idx] += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim[idx] > 1)
        dim[idx] -= 0.1;
    //  Keep angles to +/-360 degrees
    th[idx] %= 360;
    ph[idx] %= 360;

    //  Update projection
    Project(idx);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void main_special(int key, int x, int y) {
    for (int i = 0; i < 4; i++) {
        special(key, x, y, i);
    }
}

void special1(int key, int x, int y) {
    special(key, x, y, 0);
}

void special2(int key, int x, int y) {
    special(key, x, y, 1);
}

void special3(int key, int x, int y) {
    special(key, x, y, 2);
}

void special4(int key, int x, int y) {
    special(key, x, y, 3);
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y, int idx)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th[idx] = ph[idx] = 0;
    //  Toggle axes
    else if (ch == 'a' || ch == 'A')
        axes[idx] = 1 - axes[idx];
    //  Switch display mode
    else if (ch == 'm' || ch == 'M')
        mode[idx] = 1 - mode[idx];
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov[idx]--;
    else if (ch == '+' && ch<179)
        fov[idx]++;
    //  Reproject
    Project(idx);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void key1(unsigned char ch, int x, int y) {
    key(ch, x, y, 0);
}

void key2(unsigned char ch, int x, int y) {
    key(ch, x, y, 1);
}

void key3(unsigned char ch, int x, int y) {
    key(ch, x, y, 2);
}

void key4(unsigned char ch, int x, int y) {
    key(ch, x, y, 3);
}


/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width, int height, int idx)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    switch (idx) {
        case 0:
            glViewport(0, 0, width, height);
            break;
        case 1:
            glViewport(-250, 0, width, height);
            break;
        case 2:
            glViewport(120, 500, width / 2, height / 2);
            break;
        case 3:
            glViewport(0, 500, width / 2, height / 2);
            break;
    }
            
    //  Set projection
    Project(idx);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape1(int width,int height)
{
    reshape(width, height, 0);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape2(int width,int height)
{
    reshape(width, height, 1);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape3(int width,int height)
{
    reshape(width, height, 2);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape4(int width,int height)
{
    reshape(width, height, 3);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);

    glWindowPos2i = (PFNGLWINDOWPOS2IPROC) glutGetProcAddress("glWindowPos2i");

    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1000, 1000);
    main_window = glutCreateWindow("Projections");
    glutDisplayFunc(main_window_display);
    glutSpecialFunc(main_special);
    glEnable(GL_DEPTH_TEST);

    window1 = glutCreateSubWindow(main_window, 0, 0, 500, 500);
    
    //  Set callbacks
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape1);
    glutMouseFunc(mouse_func);
    glutKeyboardFunc(key1);
    glEnable(GL_DEPTH_TEST);

    window2 = glutCreateSubWindow(main_window, 500, 0, 1000, 500);
    
    //  Set callbacks
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    glutMouseFunc(mouse_func);
    glutKeyboardFunc(key2);
    glEnable(GL_DEPTH_TEST);

    window3 = glutCreateSubWindow(main_window, 0, 500, 500, 1000);
    
    //  Set callbacks
    glutDisplayFunc(display3);
    glutReshapeFunc(reshape3);
    glutMouseFunc(mouse_func);
    glutKeyboardFunc(key3);
    glEnable(GL_DEPTH_TEST);

    window4 = glutCreateSubWindow(main_window, 500, 500, 1000, 1000);
    
    //  Set callbacks
    glutDisplayFunc(display4);
    glutReshapeFunc(reshape4);
    glutMouseFunc(mouse_func);
    glutKeyboardFunc(key4);
    glEnable(GL_DEPTH_TEST);


    glutIdleFunc(idleLoop);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}
