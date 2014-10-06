#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

/* Understand that images are just numerical patterns in memory */
/* Show transfer of data from CPU space to the graphics system */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
#include <time.h>

# define PI 3.14159265358979323846

// opengl/glut info
int window_x_size = 512;
int window_y_size = 512;
int first_draw_flag = 0;

# define MAX_ROW 32
# define MAX_COL 32

int tile_pattern[MAX_ROW][MAX_COL];

# define PIXEL_ROW 512
# define PIXEL_COL 512

char pixels[PIXEL_ROW][PIXEL_COL][3];

void arc (float r, float start_deg, float end_deg ) {
    float ang;
    glBegin ( GL_POINTS );
    for ( ang = start_deg * PI / 180.0; ang < end_deg * PI / 180.0; ang = ang + PI / 180.0 ) {
        glVertex2f ( r * cos ( ang ), r * sin ( ang ) );
    }
    glEnd ( );
}

void draw_pattern () {
    int i, j;
    
    for(i = 0; i < MAX_ROW; i++) {
        for(j = 0; j < MAX_COL; j++) {
            switch(tile_pattern[i][j]) {
                case 0:
                {
                    glColor3f(0, 0, 0);
                    
                    glPushMatrix();
                    glTranslatef(0, 1, 0);
                    arc(0.5, 270.0, 360.0);
                    glPopMatrix();
                    
                    glPushMatrix();
                    glTranslatef(1, 0, 0);
                    arc(0.5, 90.0, 180.0);
                    glPopMatrix();
                }
                    break;
                case 1:
                {
                    glColor3f(0, 0, 0);
                    
                    glPushMatrix();
                    glTranslatef(0, 0, 0);
                    arc(0.5, 0.0, 90.0);
                    glPopMatrix();
                    
                    glPushMatrix();
                    glTranslatef(1, 1, 0);
                    arc(0.5, 180.0, 270.0);
                    glPopMatrix();
                }
                    break;
                default:
                    printf("got   tile_pattern[i][j] %d\n",
                           tile_pattern[i][j]);
                    break;
            }
            glTranslatef(1, 0, 0);
        }
        glTranslatef(-MAX_COL, 0, 0);
        glTranslatef(0, 1, 0);
    }
}

void putpixel (int x, int y, int r, int g, int b ) {
    char *p;
    //If the pixel is outside the range, return.
    if ( ( x < 0 ) || ( y < 0 ) ||
        ( PIXEL_COL <= x ) || ( PIXEL_ROW <= y ) )
        return;
    
    //illustrate 2D addressing of 1D memory
    p = (char *)pixels + 3*x + y*(3*PIXEL_COL);
    
    *p = r;
    p++;
    
    *p = g;
    p++;
    
    *p = b;
}

//GETPIXEL returns the color of a pixel.
void getpixel (int x, int y, int *r, int *g, int *b ) {
    char *p;
    //If the pixel is outside the range, return black.
    if ( ( x < 0 ) || ( y < 0 ) ||
        ( PIXEL_COL <= x ) || ( PIXEL_ROW <= y ) ) {
        *r = 0;
        *g = 0;
        *b = 0;
    } else {
        p = ( char * ) pixels + 3 * x + y * ( 3 * PIXEL_ROW );
        *r = (unsigned char) *p++;
        *g = (unsigned char) *p++;
        *b = (unsigned char) *p;
    }
}

//FLOODFILL recursively colors a pixel and all its neighbors.
void floodfill (int x, int y, int r, int g, int b ) {
    int r1, g1, b1;
    // Retrieve the current color of the pixel at (X,Y).
    getpixel ( x, y, &r1, &g1, &b1 );
    // Is the pixel white? If so, color it, and check its four immediate neighbors.
    if ( r1 == 255 && g1 == 255 && b1 == 255 )  {
        putpixel ( x, y, r, g, b );
        floodfill ( x+1, y, r, g, b );
        floodfill ( x-1, y, r, g, b );
        floodfill ( x, y+1, r, g, b );
        floodfill ( x, y-1, r, g, b );
    }
}

//DO_FLOODFILL chooses a color, and fills the selected region.
void do_floodfill (int x, int y ) {
    int r, g, b;
    //Choose a random 8 bit color.
    r = rand ( ) & 0xFF;
    g = rand ( ) & 0xFF;
    b = rand ( ) & 0xFF;
    //Apply that color to this pixel, and all its neighbors.
    floodfill ( x, y, r, g, b );
}

void display () {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glScalef(16, 16, 1);
    draw_pattern();
    glPopMatrix();
    
    if(first_draw_flag == 0) {
        first_draw_flag = 1;
        glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glDrawPixels(512, 512, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
    
    return;
}

void idle () {
    glutPostRedisplay();
}

void mouse (int button, int state, int x, int y ) {
    printf("got mouse %d @ %d %d\n",button,x,y);
    // map mouse y to array y
    y = 512 - y;
    
    switch(button) {
        case GLUT_LEFT_BUTTON:
            if ( state == GLUT_DOWN ) {
                do_floodfill ( x, y );
            }
            break;
    }
}

void setup () {
    //glut setup
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(window_x_size, window_y_size);
    
    glutCreateWindow("Main Window");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    
    //gl setup
    glClearColor ( 1, 1, 1, 0 );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_x_size, 0, window_y_size);
    
    //Data setup:
    int i, j;
    int seed = time(NULL);
    srand(seed);
    for(i = 0; i < MAX_ROW; i++) {
        for(j = 0; j < MAX_COL; j++) {
            tile_pattern[i][j] = rand() & 1;
        }
    }
}

int main (int argc, char **argv ) {
    glutInit ( &argc, argv );
    setup ();
    glutMainLoop ();
    return 0;
}
