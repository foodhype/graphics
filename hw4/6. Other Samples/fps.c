// ----------------------
// OpenGL cube demo.
//
// Written by Chris Halsall (chalsall@chalsall.com) for the
// O'Reilly Network on Linux.com (oreilly.linux.com).
// May 2000.
//
// Released into the Public Domain; do with it as you wish.
// We would like to hear about interesting uses.
//
// Coded to the groovy tunes of Yello: Pocket Universe.

#define PROGRAM_TITLE "O'Reilly Net: OpenGL Demo -- C.Halsall"

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>   // Always a good idea.
#include <time.h>    // For our FPS stats.

// Some global variables.

// Window IDs, window width and height.
int Window_ID;
int Window_Width = 300;
int Window_Height = 300;

// Cube position and rotation speed variables.
float X_Rot   = 0.9f;
float Y_Rot   = 0.0f;
float X_Speed = 0.0f;
float Y_Speed = 0.5f;
float Z_Off   =-5.0f;

// ------
// Frames per second (FPS) statistic variables and routine.
#define FRAME_RATE_SAMPLES 50
int FrameCount=0;
float FrameRate=0;

static void ourDoFPS() {
    static clock_t last=0;
    clock_t now;
    float delta;
    
    if (++FrameCount >= FRAME_RATE_SAMPLES) {
        now  = clock();
        delta= (now - last) / (float) CLOCKS_PER_SEC;
        last = now;
        
        FrameRate = FRAME_RATE_SAMPLES / delta;
        FrameCount = 0;
    }
}


// ------
// String rendering routine; leverages on GLUT routine.

static void ourPrintString(void *font, char *str) {
    int i,l=strlen(str);
    
    for(i=0;i<l;i++)
        glutBitmapCharacter(font,*str++);
}

// ------
// Routine which actually does the drawing
void cbRenderScene(void) {
    char buf[80]; // For our strings.
    
    // Need to manipulate the ModelView matrix to move our model around.
    glMatrixMode(GL_MODELVIEW);
    
    // Reset to 0,0,0; no rotation, no scaling.
    glLoadIdentity();
    
    // Move the object back from the screen.
    glTranslatef(0.0f,0.0f,Z_Off);
    
    // Rotate the calculated amount.
    glRotatef(X_Rot,1.0f,0.0f,0.0f);
    glRotatef(Y_Rot,0.0f,1.0f,0.0f);
    
    
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // We don't want depth-testing either.
    glEnable(GL_DEPTH_TEST);
    
    
    // OK, let's start drawing our planer quads.
    glBegin(GL_QUADS);
    
    
    // Bottom Face.  Red, 75% opaque, magnified texture
    
    glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
    glColor4f(0.9,0.2,0.2,.75); // Basic polygon color
    
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    
    // Top face; offset.  White, 50% opaque.
    
    glNormal3f( 0.0f, 1.0f, 0.0f);  glColor4f(0.5,0.5,0.5,.5);
    
    glVertex3f(-1.0f,  1.3f, -1.0f);
    glVertex3f(-1.0f,  1.3f,  1.0f);
    glVertex3f( 1.0f,  1.3f,  1.0f);
    glVertex3f( 1.0f,  1.3f, -1.0f);
    
    
    // Far face.  Green, 50% opaque, non-uniform texture cooridinates.
    
    glNormal3f( 0.0f, 0.0f,-1.0f);  glColor4f(0.2,0.9,0.2,.5);
    
    glVertex3f(-1.0f, -1.0f, -1.3f);
    glVertex3f(-1.0f,  1.0f, -1.3f);
    glVertex3f( 1.0f,  1.0f, -1.3f);
    glVertex3f( 1.0f, -1.0f, -1.3f);
    
    
    // Right face.  Blue; 25% opaque
    
    glNormal3f( 1.0f, 0.0f, 0.0f);  glColor4f(0.2,0.2,0.9,.25);
    
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    
    // Front face; offset.  Multi-colored, 50% opaque.
    
    glNormal3f( 0.0f, 0.0f, 1.0f);
    
    glColor4f( 0.9f, 0.2f, 0.2f, 0.5f);
    glVertex3f(-1.0f, -1.0f,  1.3f);
    glColor4f( 0.2f, 0.9f, 0.2f, 0.5f);
    glVertex3f( 1.0f, -1.0f,  1.3f);
    glColor4f( 0.2f, 0.2f, 0.9f, 0.5f);
    glVertex3f( 1.0f,  1.0f,  1.3f);
    glColor4f( 0.1f, 0.1f, 0.1f, 0.5f);
    glVertex3f(-1.0f,  1.0f,  1.3f);
    
    
    // Left Face; offset.  Yellow, varying levels of opaque.
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    
    glColor4f(0.9,0.9,0.2,0.0);
    glVertex3f(-1.3f, -1.0f, -1.0f);
    glColor4f(0.9,0.9,0.2,0.66);
    glVertex3f(-1.3f, -1.0f,  1.0f);
    glColor4f(0.9,0.9,0.2,1.0);
    glVertex3f(-1.3f,  1.0f,  1.0f);
    glColor4f(0.9,0.9,0.2,0.33);
    glVertex3f(-1.3f,  1.0f, -1.0f);
    
    
    // All polygons have been drawn.
    glEnd();
    
    // Move back to the origin (for the text, below).
    glLoadIdentity();
    
    // We need to change the projection matrix for the text rendering.
    glMatrixMode(GL_PROJECTION);
    
    // But we like our current view too; so we save it here.
    glPushMatrix();
    
    // Now we set up a new projection for the text.
    glLoadIdentity();
    glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
    
    // We don't want depth-testing either.
    glDisable(GL_DEPTH_TEST);
    
    // But, for fun, let's make the text partially transparent too.
    glColor4f(0.6,1.0,0.6,.75);
    
    // Now we want to render the calulated FPS at the top.
    // To ease, simply translate up.  Note we're working in screen
    // pixels in this projection.
    glTranslatef(6.0f,Window_Height - 14,0.0f);
    
    // Make sure we can read the FPS section by first placing a
    // dark, mostly opaque backdrop rectangle.
    glColor4f(0.2,0.2,0.2,0.75);
    
    glBegin(GL_QUADS);
    glVertex3f(  0.0f, -2.0f, 0.0f);
    glVertex3f(  0.0f, 12.0f, 0.0f);
    glVertex3f(140.0f, 12.0f, 0.0f);
    glVertex3f(140.0f, -2.0f, 0.0f);
    glEnd();
    
    glColor4f(0.9,0.2,0.2,.75);
    sprintf(buf,"FPS: %f F: %2d", FrameRate, FrameCount);
    glRasterPos2i(6,0);
    ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
    
    // Done with this special projection matrix.  Throw it away.
    glPopMatrix();
    
    // All done drawing.  Let's show it.
    glutSwapBuffers();
    
    // Now let's do the motion calculations.
    X_Rot+=X_Speed;
    Y_Rot+=Y_Speed;
    
    // And collect our statistics.
    ourDoFPS();
}

// ------
// Callback function called when a normal key is pressed.
void cbKeyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 113: case 81: case 27: // Q (Escape) - We're outta here.
            glutDestroyWindow(Window_ID);
            exit(0);
            break; // exit doesn't return, but anyway...
        case 115: case 83: case 32:  // F (Space) - Freeze!
            X_Speed=Y_Speed=0;
            break;
        case 114: case 82:  // R - Reverse.
            X_Speed=-X_Speed;
            Y_Speed=-Y_Speed;
            break;
        default:
            printf ("KP: No action for %d.\n", key);
            break;
    }
}

// ------
// Callback Function called when a special key is pressed.
void cbSpecialKeyPressed(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_PAGE_UP: // move the cube into the distance.
            Z_Off -= 0.05f;
            break;
            
        case GLUT_KEY_PAGE_DOWN: // move the cube closer.
            Z_Off += 0.05f;
            break;
            
        case GLUT_KEY_UP: // decrease x rotation speed;
            X_Speed -= 0.01f;
            break;
            
        case GLUT_KEY_DOWN: // increase x rotation speed;
            X_Speed += 0.01f;
            break;
            
        case GLUT_KEY_LEFT: // decrease y rotation speed;
            Y_Speed -= 0.01f;
            break;
            
        case GLUT_KEY_RIGHT: // increase y rotation speed;
            Y_Speed += 0.01f;
            break;
            
        default:
            printf ("SKP: No action for %d.\n", key);
            break;
    }
}

// ------
// Callback routine executed whenever our window is resized.  Lets us
// request the newly appropriate perspective projection matrix for
// our needs.  Try removing the gluPerspective() call to see what happens.
void cbResizeScene(int Width, int Height) {
    // Let's not core dump, no matter what.
    if (Height == 0)
        Height = 1;
    
    glViewport(0, 0, Width, Height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    
    Window_Width  = Width;
    Window_Height = Height;
}

// ------
// Does everything needed before losing control to the main
// OpenGL event loop.
void ourInit(int Width, int Height) {
    // Color to clear color buffer to.
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    
    // Depth to clear depth buffer to; type of test.
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    
    // Load up the correct perspective matrix; using a callback directly.
    cbResizeScene(Width,Height);
}

// ------
// The main() function.  Inits OpenGL.  Calls our own init function,
// then passes control onto OpenGL.
int main( int argc, char **argv) {
    glutInit(&argc, argv);
    
    // To see OpenGL drawing, take out the GLUT_DOUBLE request.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(Window_Width, Window_Height);
    
    // Open a window
    Window_ID = glutCreateWindow( PROGRAM_TITLE );
    
    // Register the callback function to do the drawing.
    glutDisplayFunc(&cbRenderScene);
    
    // If there's nothing to do, draw.
    glutIdleFunc(&cbRenderScene);
    
    // It's a good idea to know when our window's resized.
    glutReshapeFunc(&cbResizeScene);
    
    // And let's get some keyboard input.
    glutKeyboardFunc(&cbKeyPressed);
    glutSpecialFunc(&cbSpecialKeyPressed);
    
    // OK, OpenGL's ready to go.  Let's call our own init function.
    ourInit(Window_Width, Window_Height);
    
    // Pass off control to OpenGL.
    // Above functions are called as appropriate.
    glutMainLoop();
    
    return 0;
}
