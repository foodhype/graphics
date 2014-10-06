/*
 *  Example 19: Polygons
 *  '+/-' changes polygon mode
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__  // include Mac OS X verions of headers

#  include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#  include <GL/glew.h>

#  include <GL/freeglut.h>

#  include <GL/freeglut_ext.h>

#endif  // __APPLE__
int mode=0;        //  Polygon mode
double XYZ[9][3];  //  Large pentagon
double xyz[5][3];  //  Small pentagon
//  Modes
#define MODES 16
char* text[] = {"Outline","Filled Outline","Star Outline","Filled Star","Triangles and Pentagon","Quads",
    "Tesselation Outline","Tesselation Fill","Tesselation Outline with Hole","Tesselation Fill with Hole",
    "Tesselation Outline (Odd)","Tesselation Fill (Odd)","Tesselation Outline with Hole (Odd)","Tesselation Fill with Hole (Odd)",
    "Tesselation Outline with Hole (Odd)","Tesselation Fill with Hole (Odd)"};

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
 *  Report Tessalation errors
 */
void TessError(GLenum err)
{
    fprintf(stderr,"Tessellation Error: %s\n",gluErrorString(err));
    exit(1);
}

/*
 *  Generate vertex that combines vertices
 */
void TessCombine(double coords[3],double* data[4],float weight[4],double** result)
{
    *result = (double*) malloc(3 * sizeof(double));
    (*result)[0] = coords[0];
    (*result)[1] = coords[1];
    (*result)[2] = coords[2];
}

/*
 *  Tesselated star polygon
 */
void TesselatedStar(int star,int type,int hole,int rule)
{
    int k;
    //  Create new Tesselator
    GLUtesselator* tess = gluNewTess();
    //  Set polygon type (Line or Fill) and line width
    glPolygonMode(GL_FRONT_AND_BACK,type);
    if (type==GL_LINE) glLineWidth(3);
    //  Set winding rule
    gluTessProperty(tess,GLU_TESS_WINDING_RULE,rule);
    //  Set callbacks
    gluTessCallback(tess,GLU_TESS_BEGIN  ,glBegin);
    gluTessCallback(tess,GLU_TESS_END    ,glEnd);
    gluTessCallback(tess,GLU_TESS_VERTEX ,glVertex3dv);
    gluTessCallback(tess,GLU_TESS_COMBINE,TessCombine);
    gluTessCallback(tess,GLU_TESS_ERROR  ,TessError);
    //  Start polygon
    gluTessBeginPolygon(tess,NULL);
    //  Draw outside star
    if (star)
    {
        gluTessBeginContour(tess);
        for (k=0;k<5;k++)
            gluTessVertex(tess,XYZ[(2*k)%5],XYZ[(2*k)%5]);
        gluTessEndContour(tess);
    }
    else
    {
        //  Draw outside pentagon
        gluTessBeginContour(tess);
        for (k=0;k<5;k++)
            gluTessVertex(tess,XYZ[k],XYZ[k]);
        gluTessEndContour(tess);
        //  Draw inside pentagon
        gluTessBeginContour(tess);
        for (k=0;k<5;k++)
            gluTessVertex(tess,xyz[k],xyz[k]);
        gluTessEndContour(tess);
    }
    //  Draw diamond
    if (hole)
    {
        gluTessBeginContour(tess);
        for (k=5;k<9;k++)
            gluTessVertex(tess,XYZ[k],XYZ[k]);
        gluTessEndContour(tess);
    }
    //  End of polygon
    gluTessEndPolygon(tess);
    //  Delete tessalator
    gluDeleteTess(tess);
    //  Set polygon mode back to fill
    if (type==GL_LINE) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    int k;
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    //  Draw star or pentagon
    glColor3f(1,1,0);
    switch (mode)
    {
        case 0:
            break;
        case 1:
            glBegin(GL_POLYGON);
            for (k=0;k<5;k++)
                glVertex3dv(XYZ[k]);
            glEnd();
            break;
        case 2:
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            for (k=0;k<5;k++)
                glVertex3dv(XYZ[(2*k)%5]);
            glEnd();
            break;
        case 3:
            glBegin(GL_POLYGON);
            for (k=0;k<5;k++)
                glVertex3dv(XYZ[(2*k)%5]);
            glEnd();
            break;
        case 4:
            glBegin(GL_TRIANGLES);
            for (k=0;k<5;k++)
            {
                glVertex3dv(xyz[(k+4)%5]);
                glVertex3dv(XYZ[k]);
                glVertex3dv(xyz[k]);
            }
            glEnd();
            glColor3f(1,0.8,0);
            glBegin(GL_POLYGON);
            for (k=0;k<5;k++)
                glVertex3dv(xyz[k]);
            glEnd();
            break;
        case 5:
            glBegin(GL_QUADS);
            for (k=0;k<5;k++)
            {
                glVertex2f(0,0);
                glVertex3dv(xyz[(k+4)%5]);
                glVertex3dv(XYZ[k]);
                glVertex3dv(xyz[k]);
            }
            glEnd();
            glColor3f(1,0.8,0);
            glLineWidth(3);
            for (k=0;k<5;k++)
            {
                glBegin(GL_LINE_LOOP);
                glVertex2f(0,0);
                glVertex3dv(xyz[(k+4)%5]);
                glVertex3dv(XYZ[k]);
                glVertex3dv(xyz[k]);
                glEnd();
            }
            break;
        case 6:
            TesselatedStar(1,GL_LINE,0,GLU_TESS_WINDING_POSITIVE);
            break;
        case 7:
            TesselatedStar(1,GL_FILL,0,GLU_TESS_WINDING_POSITIVE);
            break;
        case 8:
            TesselatedStar(1,GL_LINE,1,GLU_TESS_WINDING_POSITIVE);
            break;
        case 9:
            TesselatedStar(1,GL_FILL,1,GLU_TESS_WINDING_POSITIVE);
            break;
        case 10:
            TesselatedStar(1,GL_LINE,0,GLU_TESS_WINDING_ODD);
            break;
        case 11:
            TesselatedStar(1,GL_FILL,0,GLU_TESS_WINDING_ODD);
            break;
        case 12:
            TesselatedStar(1,GL_LINE,1,GLU_TESS_WINDING_ODD);
            break;
        case 13:
            TesselatedStar(1,GL_FILL,1,GLU_TESS_WINDING_ODD);
            break;
        case 14:
            TesselatedStar(0,GL_LINE,1,GLU_TESS_WINDING_ODD);
            break;
        case 15:
            TesselatedStar(0,GL_FILL,1,GLU_TESS_WINDING_ODD);
            break;
        default:
            break;
    }
    
    //  Draw outline for large pentagon
    glLineWidth(1);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    for (k=0;k<5;k++)
        glVertex3dv(XYZ[k]);
    glEnd();
    //  Draw outline for small pentagon
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
    for (k=0;k<5;k++)
        glVertex3dv(xyz[k]);
    glEnd();
    //  Label verteces
    glColor3f(1,1,1);
    for (k=0;k<5;k++)
    {
        glRasterPos3dv(XYZ[k]);
        Print("%d",k);
        glRasterPos3dv(xyz[k]);
        Print("%d",k);
    }
    
    //  Display parameters
    glColor3f(1,1,1);
    glWindowPos2i(5,5);
    Print("Mode=%d [%s]",mode,text[mode]);
    //  Render the scene and make it visible
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Increase/decrease polygon mode
    else if (ch == '-' && mode>0)
        mode--;
    else if (ch == '+' && mode<MODES-1)
        mode++;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Size of world
    double dim=1.1;
    //  Ratio of the width to the height of the window
    double asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Transformation
    glOrtho(-asp*dim,+asp*dim,-dim,+dim,-dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    int k;
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Polygons");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    //  Initialize star
    for (k=0;k<5;k++)
    {
        XYZ[k][0] = Sin(72*k);
        XYZ[k][1] = Cos(72*k);
        XYZ[k][2] = 0;
        xyz[k][0] = 0.381967*Sin(72*k+36);
        xyz[k][1] = 0.381967*Cos(72*k+36);
        xyz[k][2] = 0;
    }
    //  Central diamond
    XYZ[5][0] = -0.2; XYZ[5][1] =    0; XYZ[5][2] = 0;
    XYZ[6][0] =    0; XYZ[6][1] = -0.2; XYZ[6][2] = 0;
    XYZ[7][0] = +0.2; XYZ[7][1] =    0; XYZ[7][2] = 0;
    XYZ[8][0] =    0; XYZ[8][1] = +0.2; XYZ[8][2] = 0;
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}