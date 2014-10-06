#include <stdlib.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__


typedef float point[4];

/* initial tetrahedron */
point v[]={{0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
    {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};

GLfloat theta1 = 0.0;
int n;
int mode;

void triangle( point a, point b, point c) {
    /* display one triangle using a line loop for wire frame, a single
     normal for constant shading, or three normals for interpolative (smooth) shading.
     Note that using the vertex coordinates for the  normal coordinates works here only
     because the geometry is symmetrical about the origin! */
    if (mode==0)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glEnd();
}

void normal(point p) {
    /* normalize a vector */
    double sqrt();
    float d =0.0;
    int i;
    for(i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d>0.0) for(i=0; i<3; i++) p[i]/=d;
}

void divide_triangle(point a, point b, point c, int m) {
    /* triangle subdivision using vertex numbers
     righthand rule applied to create outward pointing faces */
    point v1, v2, v3;
    int j;
    if(m>0) {
        for(j=0; j<3; j++) v1[j]=a[j]+b[j];
        normal(v1);
        for(j=0; j<3; j++) v2[j]=a[j]+c[j];
        normal(v2);
        for(j=0; j<3; j++) v3[j]=b[j]+c[j];
        normal(v3);
        divide_triangle(a, v1, v2, m-1);
        divide_triangle(c, v2, v3, m-1);
        divide_triangle(b, v3, v1, m-1);
        divide_triangle(v1, v3, v2, m-1);
    }
    else(triangle(a,b,c)); /* draw triangle at end of recursion */
}

void tetrahedron( int m) {
    /* Apply triangle subdivision to faces of tetrahedron */
    divide_triangle(v[0], v[1], v[2], m);
    divide_triangle(v[3], v[2], v[1], m);
    divide_triangle(v[0], v[3], v[1], m);
    divide_triangle(v[0], v[2], v[3], m);
}

void display(void) {
    /* Displays all three modes, side by side */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  //load an identity matrix into the MODELVIEW
    gluLookAt(0,0,6,0,0,0,0,1,0);  //postmultiply a transform that establishes the viewpoing
    glRotatef(theta1, 0,1,0);
    glColor3f (1.0, 0.0, 0.0);
    tetrahedron(n);
    glFlush();
    glutSwapBuffers();
}

void spin() {
    theta1 += 0.1; // slow rotation
    if( theta1 > 360.0 ) theta1 -= 360.0;
    glutPostRedisplay();  // generate a display event after the objects are rotated
}

void key_press( unsigned char key, int x, int y )
{
    switch( key )
    {
        case '0':
            n=0;
            glutPostRedisplay();
            break;
        case '1':
            n=1;
            glutPostRedisplay();
            break;
        case '2':
            n=2;
            glutPostRedisplay();
            break;
        case '3':
            n=3;
            glutPostRedisplay();
            break;
        case '4':
            n=4;
            glutPostRedisplay();
            break;
        case '5':
            n=5;
            glutPostRedisplay();
            break;
        case '6':
            n=6;
            glutPostRedisplay();
            break;
        case 'm':
            mode = !mode;
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            exit(0);
    }
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);  // The default viewport function
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,20);
    glMatrixMode(GL_MODELVIEW);
    display();
}

int main(int argc, char **argv) {
    n=0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("sphere");
    
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glColor3f (0.0, 0.0, 0.0);
    
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc( key_press );
    glutIdleFunc(spin);
    glutMainLoop();
}
