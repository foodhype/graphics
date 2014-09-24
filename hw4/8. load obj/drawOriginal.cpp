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

/* function prototypes */
void init	(void);
void display	(void);

const GLfloat Xmin = -1.0;
const GLfloat Xmax = 1.0;
const GLfloat Ymin = -1.0;
const GLfloat Ymax = 1.0;


GLuint ProjLoc;
GLMmodel *objmodel_ptr;

void reshape2( int w, int h ) {
    glViewport( 0, 0, w, h );
    
    GLfloat xmin = Xmin, xmax = Xmax, ymin = Ymin, ymax = Ymax;
    GLfloat aspect = GLfloat(w) / h;
    
    if ( aspect < 1.0 ) {
        ymin /= aspect;
        ymax /= aspect;
    }
    else {
        xmin *= aspect;
        xmax *= aspect;
    }
    
    mat4 ortho = Ortho2D( xmin, xmax, ymin, ymax );
    glUniformMatrix4fv( ProjLoc, 1, GL_FALSE, ortho );
}

void reshape(int width, int height) {         //  Determines Projection type
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.00, 30.0);
    // Perspective Parms( Angle, Aspect Ratio, Front Clipping, Back Clipping)
    
    gluLookAt(0.0, 2.5, 3.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
    // LookAt Parms (Eye_X, Eye_Y, Eye_Z, View_X, View_Y, View_Z, Up_X, Up_Y, UpZ)
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard( unsigned char key, int x, int y ) {
    switch ( key ) {
        case 033:
            exit( EXIT_SUCCESS );
            break;
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
        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * display:
 *
 * Display callback function.
 */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);		/* clear FB with backgd color */
    glColor3f   (0.0, 1.0, 0.0);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* double-buffer; RGB    */
	glutInitWindowSize(500, 500);		/* initialize window size     */
	glutInitWindowPosition(100, 100);	/* initialize window position */
	glutCreateWindow(argv[0]);		/* set titlebar name; create  */
	init();					/* initialize state variables */
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    
	glutMainLoop();				/* enter infinite event loop  */
	return 0;				/* ANSI C requires int main() */
}



