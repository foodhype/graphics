/*******************************************************************
 lightposition.c
 
 Rewritten from cube.c to illustrate the effect of moving the light
 source.  This program provides a light source rotating at a distance
 of 20 from the x axis and rotating about the x axis.
 
 It is based upon Angel's cube.c, in Appendix A.
 
 
 Orinal: H. Masterman
 Modified by: A. Montoya
 *********************************************************************/


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

static GLfloat theta[] = {0.0,0.0,0.0};
GLfloat theta1 = 0.0, theta2 = 0.0;  //stores two angles
int n;
int mode;
GLfloat light_position[]={0.0, 0.0, 0.0, 1.0};

void triangle( point a, point b, point c) {
    
    /* display one triangle using a line loop for wire frame, a single
     normal for constant shading, or three normals for interpolative (smooth) shading.
     Note that using the vertex coordinates for the  normal coordinates works here only
     because the geometry is symmetrical about the origin! */
    if (mode==0)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    
    if(mode==1) glNormal3fv(a);
    
    if(mode==2) glNormal3fv(a);
    glVertex3fv(a);
    if(mode==2) glNormal3fv(b);
    glVertex3fv(b);
    if(mode==2) glNormal3fv(c);
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
    
    // These 4 lines are the code that I added to get the revolving light effect
    glPushMatrix();  // Saves the glu:LookAt() matrix on the top of the stack for later recovery
    glRotatef(theta2,1,0,0); //Post multiplies a rotation about x by the incrementing variable, theta2
    glTranslatef(0,0,-20);  //Post multiplies a translation in the - z direction
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);  //Positions the light
    //Note that light_position is {0,0,0,1}, so the light is transformed by the
    //current state of the MODELVIEW matrix:  first the translation to z= -20.0, then the rotation by
    //theta, and finally, the gluLookAt.
    glPopMatrix();  // Recovers the state of MODELVIEW- now just the gluLookAt matrix
    
    glRotatef(theta1, 0,1,0);
    mode=0;  // That is, wireframe.
    tetrahedron(n);
    mode=1;  // That is, flat shading
    glTranslatef(-2.0, 0.0,0.0);
    tetrahedron(n);
    mode=2;  // Smooth shading
    glTranslatef( 4.0, 0.0,0.0);
    tetrahedron(n);
    
    glFlush();
    glutSwapBuffers();
}

void spin() {
    /* Idle callback, added to provide two incrementing variables */
    theta1 += 0.1; // slow rotation
    if( theta1 > 360.0 ) theta1 -= 360.0;
    theta2 += 1.0; // fast rotation
    if (theta2 > 360.0) theta2 -= 360.0;
    glutPostRedisplay();  // generate a display event after the objects are rotated
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);  // The default viewport function
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,20);
    glMatrixMode(GL_MODELVIEW);
    display();
}

void myinit() {
    GLfloat mat_specular[]={0.8,0.0, 0.0, 1.0};  // Red specular reflection
    GLfloat mat_diffuse[]={0.0, 0.5, 0.0, 1.0};  // Green diffuse reflection
    GLfloat mat_ambient[]={0.0, 0.0, 0.0, 1.0};  // No ambient
    GLfloat mat_shininess={6.0};  // Moderately shiny
    
    // For convenience, we set all of the lights to 1.0
    GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
    
    GLfloat light_direction[]={0.0, 0.0, -1.0, 0.0};
    
    /* set up ambient, diffuse, and specular components for light 0 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0);  //uncomment for a spotlight effect
    
    /* define material proerties for front face of all polygons */
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0);  /* enable light 0 */
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glColor3f (0.0, 0.0, 0.0);
}

int main(int argc, char **argv) {
    n=4;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("sphere");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(spin);
    glutMainLoop();
}
