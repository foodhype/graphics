/*
 *  Example 13: Solar System
 *  'm' cycles object
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

int mode;      //  Display mode
int axes=0;    //  Display axes
int th=0;      //  Azimuth of view angle
int ph=0;      //  Elevation of view angle
int zh=0;      //  Rotation
double asp=1;  //  Aspect ratio
double dim=3;  //  Size of world
GLUquadricObj* ball=NULL;

typedef struct
{
   char* name;
   char* ball;
   char* ring;
   double  r;
   double ph;
   double th;
   unsigned int balltex;
   unsigned int ringtex;
}  Planet;
const int n=11;
Planet planet[] = {
   {"Sun"    ,"sun.bmp"    ,NULL             , 109.2,   0.0,   0.0, 0,0},
   {"Mercury","mercury.bmp",NULL             ,  0.38,   0.0,   0.0, 0,0},
   {"Venus"  ,"venus.bmp"  ,NULL             ,  0.95, 180.0,   0.0, 0,0},
   {"Earth"  ,"earth.bmp"  ,NULL             ,  1.00,  23.5,   0.0, 0,0},
   {"Moon"   ,"moon.bmp"   ,NULL             ,  0.27,   6.6,   0.0, 0,0},
   {"Mars"   ,"mars.bmp"   ,NULL             ,  0.53,  25.2,   0.0, 0,0},
   {"Jupiter","jupiter.bmp",NULL             , 10.95,   3.1,   0.0, 0,0},
   {"Saturn" ,"saturn.bmp" ,"saturnrings.bmp",  9.14,  26.7,   0.0, 0,0},
   {"Uranus" ,"uranus.bmp" ,"uranusrings.bmp",  3.68,   0.0,  98.0, 0,0},
   {"Neptune","neptune.bmp",NULL             ,  3.57,  29.0,   0.0, 0,0},
   {"Pluto"  ,"pluto.bmp"  ,NULL             ,  0.89,   0.0,  90.0, 0,0},
   };
   

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
 *  Print message to stderr and exit
 */
void fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Load texture from BMP file
 */
unsigned int LoadTexBMP(char* file)
{
   unsigned int   texture;     // Texture name
   FILE*          f;           // File pointer
   char           magic[2];    // Image magic
   unsigned int   dx,dy,size;  // Image dimensions
   unsigned short nbp,bpp;     // Planes and bits per pixel
   unsigned char* image;       // Image data
   int            k;           // Counter

   //  Open file
   f = fopen(file,"rb");
   if (!f) fatal("Cannot open file %s\n",file);
   //  Check image magic
   if (fread(magic,2,1,f)!=1) fatal("Cannot read magic from %s\n",file);
   if (strncmp(magic,"BM",2)) fatal("Image magic not BMP in %s\n",file);
   //  Seek to and read header
   fseek(f,16,SEEK_CUR);
   if (fread(&dx ,4,1,f)!=1 || fread(&dy ,4,1,f)!=1 || fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1)
     fatal("Cannot read header from %s\n",file);
   //  Check image parameters
   if (nbp!=1) fatal("%s bit planes is not 1: %d\n",file,nbp);
   if (bpp!=24) fatal("%s bits per pixel is not 24: %d\n",file,bpp);
#ifndef GL_VERSION_2_0
   //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
   for (k=1;k<dx;k++);
   if (k!=dx) fatal("%s image width not a power of two: %d\n",file,dx);
   for (k=1;k<dy;k++);
   if (k!=dy) fatal("%s image height not a power of two: %d\n",file,dy);
#endif

   //  Allocate image memory
   size = 3*dx*dy;
   image = (unsigned char*) malloc(size);
   if (!image) fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
   //  Seek to and read image
   fseek(f,24,SEEK_CUR);
   if (fread(image,size,1,f)!=1) fatal("Error reading data from image %s\n",file);
   fclose(f);
   //  Reverse colors (BGR -> RGB)
   for (k=0;k<size;k+=3)
   {
      unsigned char temp = image[k];
      image[k]   = image[k+2];
      image[k+2] = temp;
   }

   //  Generate 2D texture
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   //  Copy image
   glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
   if (glGetError()) fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
   //  Scale linearly when image size doesn't match
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   //  Free image memory
   free(image);
   //  Return texture name
   return texture;
}
   
/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   gluPerspective(60,asp,1,4*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=1.2;
   //  Eye position
   double Ex = -2*dim*Cos(ph);
   double Ey = +2*dim*Sin(ph);
   double Ez = 0;
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   //  Draw scene
   glEnable(GL_DEPTH_TEST);
   //  Rotate Z up and inclined th
   glRotated(-90,1,0,0);
   glRotated(th,1,0,0);

   /*
    *  Draw planet
    */
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   //  Surface texture
   glBindTexture(GL_TEXTURE_2D,planet[mode].balltex);
   //  Rotation around spin axis
   glRotated(zh,0,0,1);
   //  Solid
   gluQuadricDrawStyle(ball,GLU_FILL);
   //  Calculate normals
   gluQuadricNormals(ball,GLU_SMOOTH);
   //  Apply Textures
   gluQuadricTexture(ball,1);
   //  Draw the ball
   gluSphere(ball,1.0,36,36);

   /*
    *  Draw rings for planets that have them
    *  We naively assume the ring width equals the radius
    */
   if (planet[mode].ring)
   {
      int k;
      //  Make rings transparent grey
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
      glColor3f(0.5,0.5,0.5);
      //  Set ring texture
      glBindTexture(GL_TEXTURE_2D,planet[mode].ringtex);
      //  Draw ring plane
      glBegin(GL_QUAD_STRIP);
      for (k=0;k<=360;k+=2)
      {
         glTexCoord2f(1,0);glVertex2d(1*Cos(k),1*Sin(k));
         glTexCoord2f(0,0);glVertex2d(2*Cos(k),2*Sin(k));
      }
      glEnd();
      glDisable(GL_BLEND);
   }

   /*
    *  Draw axes - no textures from here
    */
   glDisable(GL_TEXTURE_2D);
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
   Print("Angle=%d,%d  Dim=%.1f Object=%s",th,ph,dim,planet[mode].name);
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void SetMode(int k)
{
   mode = k;
   th = planet[k].th;
   ph = planet[k].ph;
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
      th = planet[mode].th;
      ph = planet[mode].ph;
   }
   //  Toggle texture mode
   else if (ch == 'm')
      SetMode((mode+1)%n);
   else if (ch == 'M')
      SetMode((mode+n-1)%n);
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   else if (ch == 'p')
       dim += 0.1;
   else if (ch == 'P')
       dim -= 0.1;
    
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   zh = 15*glutGet(GLUT_ELAPSED_TIME)/1000.0;
   glutPostRedisplay();
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
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
   glutCreateWindow("Draped Textures");
   glutFullScreen();
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Allocate quadric for ball
   ball = gluNewQuadric();
   //  Load textures
   for (k=0;k<n;k++)
   {
      planet[k].balltex = LoadTexBMP(planet[k].ball);
      planet[k].ringtex = planet[k].ring ? LoadTexBMP(planet[k].ring) : 0;
   }
   SetMode(3);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
