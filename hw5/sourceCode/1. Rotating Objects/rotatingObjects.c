/*  Light1.c   This programs draws a simple Solar system that animates
 constantly
 */

#include <math.h>
#include <stdlib.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

int mode;
static void Init() {                                 // Initialization routine
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};     // Define highlight properties
    GLfloat mat_shininess[]= {50.0};                   // Define shininess of surface
    GLfloat light_position[] = {10.0, 40.0, 1.0, 0.0}; // Define light source position
    glClearColor(0.3, 0.3, 0.3, 0.0);                  // Set background to gray
    
    glShadeModel(GL_FLAT);                           // Smooth transitions between edges
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Set material properties
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Set material properties
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // Set light properties
    glColorMaterial(GL_FRONT,GL_DIFFUSE);                // Set Color Capability
    glEnable(GL_DEPTH_TEST); 		       // Enable depth buffering
    
    mode = 1;
}

static void Reshape(int width, int height) {         //  Determines Projection type
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.00, 30.0);
    // Perspective Parms( Angle, Aspect Ratio, Front Clipping, Back Clipping)
    
    gluLookAt(0.0, 2.5, 3.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
    // LookAt Parms (Eye_X, Eye_Y, Eye_Z, View_X, View_Y, View_Z, Up_X, Up_Y, UpZ)
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadIdentity();
}

static double x; // This value will be remembered each time display is called


static void display() {    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear Color and Depth
    glPushMatrix();              		// Begin drawing image
        glRotatef( x,0.0,1.0,0.0);  	// Rotate
        glColor3f(1.0,0.0, 0.0);		// Set Drawing Color to Red
        if (!mode)
            glutSolidSphere(0.7,30,20);		// Draw Sphere(Radius, Slices, Stacks)
        else
            glutWireSphere(0.7,30,20);		// Draw Sphere(Radius, Slices, Stacks)
        
        glPushMatrix();			  // Draw orbiting sphere
            glTranslatef(0.5,0.0,-1.0);	  // Move away from origin
            glColor3f(1.0,1.0,0.0);             // Set color to yellow
            if (!mode)
                glutSolidSphere(0.3,20,16);	  // Draw Sphere
            else
                glutWireSphere(0.3,20,16);	  // Draw Sphere
            glPushMatrix();			     // Draw sphere
            glRotatef( x, 0.0, 1.0, 0.0);	     // Rotate
            glTranslatef(0.3,0.0,-0.5);	     // Move reference point
            glColor3f(0.0,0.0,1.0);		     // Color Blue
            if (!mode)
                glutSolidSphere(0.2,20,16);	     // Draw Sphere
            else
                glutWireSphere(0.2,20,16);	     // Draw Sphere
        glPopMatrix();			  // Return to former reference
        glPopMatrix();			// Return to first refernce
        glPushMatrix();			  // Draw orbiting sphere
            glTranslatef(-0.5,0.0,1.0);	  // Move reference poit
            glColor3f(0.0,1.0,0.0);		  // Set color to green
            if (!mode)
                glutSolidSphere(0.3,20,16);	  // Draw sphere
            else
                glutWireSphere(0.3,20,16);	  // Draw sphere
        glPopMatrix();			// Return to first reference
    glPopMatrix();		      // Return to base reference
    glFlush();			      // Flush drawing routines
    glutSwapBuffers();		      // Swap buffers
}

void spin() {
    x += 1.0; // slow rotation
    if( x > 360.0 ) x -= 360.0;
    glutPostRedisplay();  // generate a display event after the objects are rotated
}

void key_press( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'a':// Enable color
            glEnable(GL_COLOR_MATERIAL);
            glutPostRedisplay();
            break;
        case 'z':// Disable color
            glDisable(GL_COLOR_MATERIAL);
            glutPostRedisplay();
            break;
            
        case 's':// Enable lighting for surfaces
            glEnable(GL_LIGHTING);
            glutPostRedisplay();
            break;
        case 'x':// Disable lighting for surfaces
            glDisable(GL_LIGHTING);
            glutPostRedisplay();
            break;
            
        case 'd':// Enable light source
            glEnable(GL_LIGHT0);
            glutPostRedisplay();
            break;
        case 'c':// Disable light source
            glDisable(GL_LIGHT0);
            glutPostRedisplay();
            break;
            
        case 'f': // Enable depth buffering
            glEnable(GL_DEPTH_TEST);
            glutPostRedisplay();
            break;
        case 'v': // Disable depth buffering
            glDisable(GL_DEPTH_TEST);
            glutPostRedisplay();
            break;
            
        case 'g':
            glShadeModel(GL_SMOOTH);
            glutPostRedisplay();
            break;
        case 'b':
            glShadeModel(GL_FLAT);
            glutPostRedisplay();
            break;
            
        case 'm':
            mode = !mode;
            glutPostRedisplay();
            break;
        case 'q':
            exit(0);
    }
}

int main(int argc, char** argv) {	// Run program with possible parameters
    // "glut" Initialization Functions rather than "aux" versions
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Initialize modes
    glutInitWindowSize( 550, 550);
    glutInitWindowPosition( 50, 50);
    glutCreateWindow( argv[0]); // Note that arg[0] is name of program (Window name)
    Init();
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(key_press);
    glutIdleFunc(spin);
    glutMainLoop();
    return 0;
}
