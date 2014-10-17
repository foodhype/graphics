#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <sys/time.h>


static struct timeval last_idle_time;
static double x;
unsigned int texture[10];


static void Init() {
    glEnable(GL_DEPTH_TEST);
}


void fatal(const char * format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}


static void Reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.00, 30.0);
    gluLookAt(0.0, 2.5, 5.0, 0.0, -0.5, -1.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void AnimateScene(void) {
    float dt;
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
    // Animate the scene by Increment 2 degrees each call
    x+=2;
    // Save time_now for next time
    last_idle_time = time_now;
    // Force redraw
    glutPostRedisplay();
}


unsigned int LoadTexBMP(char* file)
{
    // Loading texture function was adapted from sample code in Transparency.
    FILE* f;
    char magic[2];
    unsigned int dx, dy, size;
    unsigned short nbp,bpp;
    unsigned char* image;
    int k;
    unsigned int texture;

    //  Open file
    f = fopen(file,"rb");
    if (!f) {
        fatal("Cannot open file %s\n",file);
    }
    //  Check image magic
    if (fread(magic,2,1,f)!=1) {
        fatal("Cannot read magic from %s\n", file);
    }
    if (strncmp(magic,"BM",2)) {
        fatal("Image magic not BMP in %s\n", file);
    }

    //  Seek to and read header
    fseek(f,16,SEEK_CUR);
    if (fread(&dx, 4, 1, f) != 1 ||
            fread(&dy, 4, 1, f) != 1 ||
            fread(&nbp, 2, 1, f) != 1 ||
            fread(&bpp, 2, 1, f) != 1) {
        fatal("Cannot read header from %s\n",file);
    }

    //  Check image parameters
    if (nbp != 1) {
        fatal("%s bit planes is not 1: %d\n", file, nbp);
    }
    if (bpp != 24) {
        fatal("%s bits per pixel is not 24: %d\n", file, bpp);
    }
   
    //  Allocate image memory
    size = 3 * dx * dy;
    image = (unsigned char*) malloc(size);
    if (!image) {
        fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
    }
    //  Seek to and read image
    fseek(f, 24, SEEK_CUR);
    if (fread(image, size, 1, f) != 1) {
        fatal("Error reading data from image %s\n",file);
    }
    fclose(f);
    //  Reverse colors (BGR -> RGB)
    for (k = 0; k < size; k += 3) {
        unsigned char temp = image[k];
        image[k]   = image[k+2];
        image[k+2] = temp;
    }

    //  Generate 2D texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //  Copy image
    glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
    if (glGetError()) {
        fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
    }
    //  Scale linearly when image size doesn't match
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    // Free image memory
    free(image);
    // Return texture name
    return texture;
}


void drawCube(GLfloat aSize) {
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-aSize, -aSize, aSize);
    glTexCoord2f(1, 0); glVertex3f(aSize, -aSize, aSize);
    glTexCoord2f(1, 1); glVertex3f(aSize, aSize, aSize);
    glTexCoord2f(0, 1); glVertex3f(-aSize, aSize, aSize);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(aSize, -aSize, -aSize);
    glTexCoord2f(1, 0); glVertex3f(-aSize, -aSize, -aSize);
    glTexCoord2f(1, 1); glVertex3f(-aSize, aSize, -aSize);
    glTexCoord2f(0, 1); glVertex3f(aSize, aSize, -aSize);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(aSize, -aSize, aSize);
    glTexCoord2f(1, 0); glVertex3f(aSize, -aSize, -aSize);
    glTexCoord2f(1, 1); glVertex3f(aSize, aSize, -aSize);
    glTexCoord2f(0, 1); glVertex3f(aSize, aSize, aSize);
    glEnd();
   
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-aSize, -aSize, -aSize);
    glTexCoord2f(1, 0); glVertex3f(-aSize, -aSize, aSize);
    glTexCoord2f(1, 1); glVertex3f(-aSize, aSize, aSize);
    glTexCoord2f(0, 1); glVertex3f(-aSize, aSize, -aSize);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-aSize, aSize, aSize);
    glTexCoord2f(1, 0); glVertex3f(aSize, aSize, aSize);
    glTexCoord2f(1, 1); glVertex3f(aSize, aSize, -aSize);
    glTexCoord2f(0, 1); glVertex3f(-aSize, aSize, -aSize);
    glEnd();
   
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-aSize, -aSize, -aSize);
    glTexCoord2f(1, 0); glVertex3f(aSize, -aSize, -aSize);
    glTexCoord2f(1, 1); glVertex3f(aSize, -aSize, aSize);
    glTexCoord2f(0, 1); glVertex3f(-aSize, -aSize, aSize);
    glEnd();
}


static void display() {
    // Minecraft planets

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // Clear Color and Depth
    // Enable textures.
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // Enable blending
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPushMatrix(); // Begin drawing image
        glRotatef(x, 0.0, 1.0, 0.0); // rotate
        glColor3f(1.0, 1.0, 0.0); // Set drawing color to yellow
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        drawCube(0.5); // Draw sun
        glutSolidCube(0.5);
        glPushMatrix(); // Draw Mercury
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glTranslatef(1.0, 0.0, -0.8); // Move away from origin
            glColor3f(1.0, 0.0, 0.0); // Set color to red
            glRotatef(x, 0.0, 0.0001, 0.0);
            drawCube(0.15);
        glPopMatrix(); // Return to first refernce

        glPushMatrix();	// Venus
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            glTranslatef(0.5, 0.2, -1.4); // Move away from origin
            glColor3f(1.0, 0.2, 0.0); // Set color to orange-red
            glRotatef(x, 0.0, 0.0001, 0.0);
            drawCube(0.23);
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, texture[8]);
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -4.0);
                glColor3f(0.8, 0.0, 5.0);
                drawCube(0.2);
            glPopMatrix();			  // Return to former reference
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, texture[8]);
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -3.5);
                glColor3f(1.0, 0.0, 8.0);
                drawCube(0.2);
            glPopMatrix(); // Return to first refernce
        glPopMatrix(); // Return to first refernce

        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[3]);
            glTranslatef(0.5, -0.5, 1.8); // Move away from origin
            glColor3f(0.0, 0.0, 1.0); // Set color to blue
            glRotatef(x, 0.0, 0.0001, 0.0);
            drawCube(0.3); // Draw Earth
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, texture[8]);
                glRotatef(x, 0.0, 1.0, 0.0); // Rotate
                glTranslatef(0.3, 0.0, -0.5); // Move reference point
                glColor3f(1.0, 1.0, 1.0); // set color to gray
                drawCube(0.1);
            glPopMatrix(); // Return to former reference
        glPopMatrix(); // Return to first refernce
        glPushMatrix();	// Draw fake planet
            glBindTexture(GL_TEXTURE_2D, texture[4]);
            glTranslatef(2.2, -0.5, 1.8); // Move away from origin
            glColor3f(1.0, 0.0, 1.0); // Set color to magenta
            glRotatef(x, 0.0, 0.0001, 0.0);
            drawCube(0.3);
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, texture[5]);
                glRotatef(x, 0.0, 1.0, 0.0);
                glTranslatef(0.3, 0.0, -5.0);
                glColor3f(0.5, 0.8, 0.5);
                drawCube(0.12);
                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, texture[6]);
                    glRotatef(x, 0.0, 1.0, 0.0);
                    glTranslatef(0.3, 0.0, -1.0);
                    glColor3f(1.0, 0.0, 8.0);
                    drawCube(0.2);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();			
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    

    glFlush();
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 550, 550);
    glutInitWindowPosition( 50, 50);
    glutCreateWindow(argv[0]);
    Init();
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutIdleFunc(AnimateScene);

    texture[0] = LoadTexBMP("Planets/sun.bmp");
    texture[1] = LoadTexBMP("Planets/mercury.bmp");
    texture[2] = LoadTexBMP("Planets/venus.bmp");
    texture[3] = LoadTexBMP("Planets/earth.bmp");
    texture[4] = LoadTexBMP("Planets/mars.bmp");
    texture[5] = LoadTexBMP("Planets/jupiter.bmp");
    texture[6] = LoadTexBMP("Planets/saturn.bmp");
    texture[7] = LoadTexBMP("Planets/uranus.bmp");
    texture[8] = LoadTexBMP("Planets/moon.bmp");

    glutMainLoop();
    return 0;
}
