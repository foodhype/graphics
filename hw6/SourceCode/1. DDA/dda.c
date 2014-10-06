/*
 * This is a simple, introductory OpenGL program.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#define NOLINE		0
#define DDA			1
#define GL			2

/*****************************************************************/

void init (void);
void reshape (int, int);
void display (void);
void menu (void);
void dda_line (int, int, int, int);
void gl_line (int, int, int, int);
void mouse (int, int, int, int);
void keyboard (unsigned char, int, int);

/*****************************************************************/
int startX, startY, stopX, stopY;
int linetype, pointType, height = 400;

/*
 * Declare initial window size, position, and display mode
 * Call initialization routines.
 * Register callback functions to display graphics, reshape
 * window, and handle mouse and keyboard events.
 * Enter main loop and process events.
 */
int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (400, height);
    glutInitWindowPosition (400, 350);
    glutCreateWindow ("Scan Conversion Algorithms");
    
    glClearColor (1.0, 1.0, 1.0, 0.0);
    
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutMouseFunc (mouse);
    glutKeyboardFunc (keyboard);
    
    menu ();
    
    glutMainLoop ();
    return (0);   /* ANSI C requires main to return int. */
}

/*****************************************************************/

void reshape (int w, int h) {
    height = h;
    /* This routine is called when the initial GL window is created
     and when the window is resized.  It is followed in execution
     by the 'display' routine. */
    
    /* initialize viewing values  */
    glViewport (0, 0, (GLint) w, (GLint) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0.0, (GLdouble) w, 0.0, (GLdouble) h, -1.0, 1.0);
}

void display (void) {
    /* clear all pixels  */
    glClear (GL_COLOR_BUFFER_BIT);
    if (linetype == DDA)
        dda_line (startX, startY, stopX, stopY);
    else if (linetype == GL)
        gl_line (startX, startY, stopX, stopY);
    glutSwapBuffers();
}
void menu (void) {
    /* This menu will be printed in the terminal window you used
     to invoke the program.  However, when typing a menu option
     you MUST do so with the GL window in focus, NOT the
     terminal window. Then, if subsequent parameters are required,
     use the terminal window(e.g. x0, y0, x1, y1). */
    
    printf ("\nMenu\n----\n");
    printf ("1)  DDA Line\n");
    printf ("2)  GL Line\n");
    printf ("3)  Clear Screen\n");
    /* Pressing the right mouse button will also terminate the program. */
    printf ("q)  Quit\n");
    printf ("\nPlease enter your choice with the GL window in focus.\n");
}

void dda_line (int x0, int y0, int x1, int y1) {
    int   x;
    float y = y0;
    float m = (y - y1) / (x0 - x1);
    int i;
    
    /* this implementation only handles abs(slope) <= 1 */
    assert (m >= -1 && m <= 1 && x0 <= x1);
    
    glBegin (GL_POINTS);
    glColor3ub (255, 0, 0);     /* red */
    for (x = x0; x <= x1; x++) {
        glVertex2i (x, (GLint) (y + 0.5));
        y += m;
    }
    glEnd ();
}

void gl_line (int x0, int y0, int x1, int y1) {
    glColor3ub (0, 255, 0); /* green */
    glBegin (GL_LINES);
        glVertex2i (x0, y0);
        glVertex2i (x1, y1);
    glEnd ();
}

void mouse (int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                printf ("x = %d, y = %d\n", x, height-y);
                if (pointType == 0) {
                    startX = x;
                    startY = height-y;
                    pointType = 1;
                } else {
                    stopX = x;
                    stopY = height-y;
                    pointType = 0;
                    glutPostRedisplay();
                }
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                exit (0);
            break;
        default:
            break;
    }
}

void keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
            menu ();
            break;
        case 'q':
            exit (0);
            break;
        case '1':
            pointType = 0;
            linetype = DDA;
            break;
        case '2':
            pointType = 0;
            linetype = GL;
            break;
        case '3':
            linetype = NOLINE;
            break;
        default:
            break;
    }
}
