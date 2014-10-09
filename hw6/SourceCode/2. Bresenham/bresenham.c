#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <stdio.h>

int startX, startY, stopX, stopY;
int linetype, pointType, height = 500;

void draw_pixel(int ix, int iy) {
    glBegin(GL_POINTS);
    glVertex2i( ix, iy);
    glEnd();
}

void bres(int x1,int y1,int x2,int y2) {
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x,y;
    
    dx = x2 - x1;
    dy = y2 - y1;
    
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    incx = 1;
    if(x2 < x1) incx = -1;
    incy = 1;
    if(y2 < y1) incy = -1;
    x=x1;
    y=y1;
    
    if(dx > dy) {
        draw_pixel(x,y);
        e = 2*dy - dx;
        inc1 = 2*( dy -dx);
        inc2 = 2*dy;
        for(i = 0; i < dx; i++) {
            if(e >= 0) {
                y += incy;
                e += inc1;
            }
            else e += inc2;
            x += incx;
            draw_pixel(x,y);
        }
    } else {
        draw_pixel(x,y);
        e = 2*dx - dy;
        inc1 = 2*( dx - dy);
        inc2 = 2*dx;
        for(i = 0; i < dy; i++) {
            if(e >= 0) {
                x += incx;
                e += inc1;
            }
            else e += inc2;
            y += incy;
            draw_pixel(x,y);
        }
    }
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    bres(startX, startY, stopX, stopY);
    glFlush();
}

void myinit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}

int main(int argc, char** argv) {
    /* standard GLUT initialization */
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(height,height); /* 500x500 pixel window */
    glutInitWindowPosition(0,0); /* place window top left on display */
    glutCreateWindow("Bresenham's Algorithm"); /* window title */
    glutDisplayFunc(display); /* display callback invoked when window opened */
    glutMouseFunc (mouse);
    myinit(); /* set attributes */
    glutMainLoop(); /* enter event loop */
}
