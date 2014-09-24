/*
 
Program: picktriangle.cpp

Adapted from SGI demo select.c to demonstrate picking by the mouse

  by H. Masterman

  September 2003
  Modified by HMasterman September 2012

 */

#define PI 3.14
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;
float radius = 20.0;
float x,y,z;
float theta, phi;


/* draw a triangle with vertices at (x1, y1), (x2, y2) 
 * and (x3, y3) at z units away from the origin.
 */
void drawTriangle (GLfloat x1, GLfloat y1, GLfloat x2, 
    GLfloat y2, GLfloat x3, GLfloat y3, GLfloat z)
{
   glBegin (GL_TRIANGLES);
   glVertex3f (x1, y1, z);
   glVertex3f (x2, y2, z);
   glVertex3f (x3, y3, z);
   glEnd ();
}

/* draw a rectangular box with these outer x, y, and z values */
void drawViewVolume (GLfloat x1, GLfloat x2, GLfloat y1, 
                     GLfloat y2, GLfloat z1, GLfloat z2)
{
   glColor3f (1.0, 1.0, 1.0);
   glBegin (GL_LINE_LOOP);
   glVertex3f (x1, y1, -z1);
   glVertex3f (x2, y1, -z1);
   glVertex3f (x2, y2, -z1);
   glVertex3f (x1, y2, -z1);
   glEnd ();

   glBegin (GL_LINE_LOOP);
   glVertex3f (x1, y1, -z2);
   glVertex3f (x2, y1, -z2);
   glVertex3f (x2, y2, -z2);
   glVertex3f (x1, y2, -z2);
   glEnd ();

   glBegin (GL_LINES);	/*  4 lines	*/
   glVertex3f (x1, y1, -z1);
   glVertex3f (x1, y1, -z2);
   glVertex3f (x1, y2, -z1);
   glVertex3f (x1, y2, -z2);
   glVertex3f (x2, y1, -z1);
   glVertex3f (x2, y1, -z2);
   glVertex3f (x2, y2, -z1);
   glVertex3f (x2, y2, -z2);
   glEnd ();
}

/* drawScene draws 4 triangles and a wire frame
 * which represents the viewing volume.
 */
void drawScene (void)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective (40.0, 4.0/3.0, 5.0, 30.0);

   glMatrixMode (GL_MODELVIEW);

   x = radius * cos (theta) * cos (phi)+ 2.5;
	y = radius * sin (theta)+2.5;
	z = -radius * cos (theta)* sin(phi)-5.0;
 glLoadIdentity ();
	gluLookAt(x,y,z,2.50,2.50,-5.0,0,1,0);  //multiply viewing component of transformation
  
   
   glColor3f (0.0, 1.0, 0.0);	/*  green triangle	*/
   drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);
   glColor3f (1.0, 0.0, 0.0);	/*  red triangle	*/
   drawTriangle (2.0, 7.0, 3.0, 7.0, 2.5, 8.0, -5.0);
   glColor3f (1.0, 1.0, 0.0);	/*  yellow triangle	*/
   drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -1.0);
   glColor3f(0.0,0.0,1.0);  // blue triangle  
   drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -9.0);
   drawViewVolume (0.0, 5.0, 0.0, 5.0, 0.0, 10.0);
}

// processHits prints out the contents of the selection array
 
void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;

   cout<<"Total number of hits was: "<<hits<<endl;
   ptr = (GLuint *) buffer;   for (i = 0; i < hits; i++) {	/*  for each hit  */
      names = *ptr;
      
	  cout<<endl<< "This hit returned "<< names <<" names"<<endl;
	  ptr++;
      cout <<"z1 is " << (float) *ptr/0xffffffff <<endl;
	  ptr++;
      cout <<"z2 is " << (float) *ptr/0xffffffff <<endl;
	  ptr++;
   
      for (j = 0; j < names; j++) {	/*  for each name */
          cout << "Name " <<j <<" is "<<*ptr<<endl;
		  ptr++;
      }

   }
}


#define BUFSIZE 512

void init (void) 
{
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}


void dopick(int button, int state, int x, int y)
{
   GLuint selectBuf[BUFSIZE];
   GLint hits;
   GLint viewport[4];

//pick when the left button is pressed down
   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
   {
		cout << "Doing a pick." << endl;
		glGetIntegerv (GL_VIEWPORT, viewport);  //get viewport parameteres

		glSelectBuffer (BUFSIZE, selectBuf);  // sets up a selection buffer
	    glRenderMode (GL_SELECT);  // enters selection mode

		glInitNames();  // initialize the name stack
		glPushName(0);  // push a name on the stack

   
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();
		//first multiply a "pick matrix" on the projection - so it acts last
		gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3]- y), 
               5.0, 5.0, viewport);  // viewport[3] contains the y dimension of the viewport
		gluPerspective (40.0, 4.0/3.0, 5.0, 30.0); //match projection in drawscene()

		//draw the objects with proper names in the name stack

		glLoadName(1);
		drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);

		glLoadName(2);
		drawTriangle (2.0, 7.0, 3.0, 7.0, 2.5, 8.0, -5.0);

		glLoadName(3);
		drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -1.0);

		glLoadName(4);
		drawTriangle (2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -9.0);

		hits = glRenderMode (GL_RENDER);
		processHits (hits, selectBuf);

		glPopMatrix();
   }
} 

void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   
   drawScene ();
   glFlush();
}

void special (int key_pressed, int x, int y)
{
	if (key_pressed == GLUT_KEY_LEFT)
	{
		phi -= 0.1;  //radians
		glutPostRedisplay();
		return;
	}
	if (key_pressed == GLUT_KEY_RIGHT)
	{
		phi += 0.1;  //radians
		glutPostRedisplay();
		return;
	}
	if (key_pressed == GLUT_KEY_UP)
	{
		if (theta < (PI/2 - 0.1)) 
			theta += 0.1;
		glutPostRedisplay();
		return;
	}
	if (key_pressed == GLUT_KEY_DOWN)
	{
		 if (theta > (-PI/2 + 0.1)) 
		theta -= 0.1;
		glutPostRedisplay();
		return;
	}

	if (key_pressed == GLUT_KEY_PAGE_UP)
	{
		radius += 0.5;
		glutPostRedisplay();
		return;
	}

	if (key_pressed == GLUT_KEY_PAGE_DOWN)
	{
		radius -= 0.5;
		glutPostRedisplay();
		return;
	}
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

/*  Main Loop  */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (700, 700);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMouseFunc(dopick);
   glutMainLoop();
   return 0; 
}
