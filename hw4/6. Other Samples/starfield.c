/* 
    starfield.c
    Nate Robins, 1997

    An example of starfields in OpenGL.

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

enum {
    SCROLL_LEFT = 1,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN
} type = SCROLL_RIGHT;


typedef struct _star {
    float x, y;
    float vx, vy;
} star;


star* stars = NULL;
int num_stars = 150;


void
reshape(int width, int height)
{
    int i = 0;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(255, 255, 255);

    for (i = 0; i < num_stars; i++) {
	stars[i].x = rand() % width;
	stars[i].y = rand() % height;
	stars[i].vx = rand() / (float)RAND_MAX * 5 + 2;
	stars[i].vy = 0;
    }
}

void
display(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < num_stars; i++) {
	stars[i].x += stars[i].vx;
	if (stars[i].x < glutGet(GLUT_WINDOW_WIDTH)) {
	    glBegin(GL_LINE_STRIP);
	    glColor3ub(0, 0, 0);
	    glVertex2i(stars[i].x-stars[i].vx*4, stars[i].y);
	    glColor3ub(255, 255, 255);
	    glVertex2i(stars[i].x, stars[i].y);
	    glEnd();
	} else {
	    stars[i].x = 0;
	}
    }
    
    glutSwapBuffers();
}

void idle(void) {
    glutPostRedisplay();
}

void bail(int code) {
    free(stars);
    exit(code);
}

void keyboard(unsigned char key, int x, int y) {
    static int old_x = 50;
    static int old_y = 50;
    static int old_width = 320;
    static int old_height = 320;

    switch (key) {
    case 27:
        bail(0);
	break;
    case 'w':
        glutPositionWindow(old_x, old_y);
        glutReshapeWindow(old_width, old_height);
	break;
    case 'f':
	if (glutGet(GLUT_WINDOW_WIDTH) < glutGet(GLUT_SCREEN_WIDTH)) {
	    old_x = glutGet(GLUT_WINDOW_X);
	    old_y = glutGet(GLUT_WINDOW_Y);
	    old_width = glutGet(GLUT_WINDOW_WIDTH);
	    old_height = glutGet(GLUT_WINDOW_HEIGHT);
	    glutFullScreen();
	}
	break;
    }
}

int main(int argc, char** argv) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);

    glutCreateWindow("Starfield");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    if (argc > 1) {
	if (strcmp(argv[1], "-h") == 0) {
	    fprintf(stderr, "%s [stars]\n", argv[0]);
	    exit(0);
	}
	sscanf(argv[1], "%d", &num_stars);
    }      

    stars = (star*)malloc(sizeof(star) * num_stars);

    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
