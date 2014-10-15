/* ======================================================================
 *
 * draw.c - Draw a filled polygon.
 *
 * Written by: George Wolberg, 2012
 * Modified by: Alejandro Montoya 2014
 * ======================================================================
 */

#ifdef __APPLE__  // include Mac OS X verions of headers
    #  include <OpenGL/OpenGL.h>
    #  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
    #  include <GL/glew.h>
    #  include <GL/freeglut.h>
    #  include <GL/freeglut_ext.h>
#endif  // __APPLE__

/* function prototypes */
void init	(void);
void display	(void);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * main:
 *
 * Initialize window info, state variables, and callback functions.
 * Enter main loop and process events.
 */
int main(int argc, char** argv) {
    glutInit( &argc, argv );
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* double-buffer; RGB    */
	glutInitWindowSize(640, 640);		/* initialize window size     */
	glutInitWindowPosition(100, 100);	/* initialize window position */
	glutCreateWindow("CS354 Computer Graphics");		/* set titlebar name; create  */
	init();					/* initialize state variables */
	glutDisplayFunc(display);		/* display callback function  */
	glutMainLoop();				/* enter infinite event loop  */
	return 0;				/* ANSI C requires int main() */
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * init: Initialize state variables.
 */
void
init(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	/* set background color */
	glColor3f   (0.0, 1.0, 1.0);		/* set foreground color */

	/* initialize viewing values; setup unit view cube */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * display: Display callback function.
 */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);		/* clear FB with backgd color */
	glBegin(GL_POLYGON);			/* begin polygon definition   */
		glVertex3f(0.25, 0.25, 0.0);	/* 1st  vertex		      */
		glVertex3f(0.75, 0.25, 0.0);	/* 2nd  vertex		      */
		glVertex3f(0.75, 0.75, 0.0);	/* 3rd  vertex		      */
		glVertex3f(0.25, 0.75, 0.0);	/* last vertex		      */
	glEnd();				/* end polygon definition     */
   	glFlush();				/* draw to (off-screen) buffer*/
	glutSwapBuffers();			/* swap buffer to see drawing */
}
