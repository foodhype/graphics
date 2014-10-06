/*================================================================================
 Point Source Per Pixel Lighting Demo
 Adapted  by H Masterman May 2010
 From Lighthouse 3D GLSL demo program
 =================================================================================*/


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>

#endif  // __APPLE__


#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#define PI 3.14159
using namespace std;
int width  = 700;
int height = 700;
double light_angle;
float LightLocation[4];
GLuint PerPixelShader;  // designator for the per pixel shader program

char* readShaderSource(const char* shaderFile) {
    //struct stat statBuf;
    FILE* fp = fopen(shaderFile, "r");
    char* buf;
    long size;
    
    //stat(shaderFile, &statBuf);
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    cout<<"size is "<<size<<endl;
    fseek(fp,0L,SEEK_SET);
    buf = (char*) calloc((size + 1) , sizeof(char));
    fread(buf, 1, size, fp);
    //for (long i=0; i<size;i++) cout<<buf[i];
    //buf[size] = '\0';
    fclose(fp);
    return buf;
}


int printOpenGLError() {
    GLenum  glErr;
    int     retCode = 0;
    char    *err;
    
    // returns 1 if an OpenGL error occurred, 0 otherwise.
    glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        err = (char*)gluErrorString(glErr);
        cout<<"error: "<<err<<endl;
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void printShaderInfoLog(GLuint shader) {
    int infologLen = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    printOpenGLError();  // Check for OpenGL errors
    
    if (infologLen > 0) {
        infoLog = (GLchar*) malloc(infologLen);
        if (infoLog == NULL) {
            printf("ERROR: Could not allocate InfoLog buffer\n");
            exit(1);
        }
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
        printf("InfoLog:\n%s\n\n", infoLog);
        free(infoLog);
    }
    printOpenGLError(); // Check for OpenGL errors
}

void printProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 0) {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

GLint getUniLoc(GLuint program, const GLchar *name) {
    GLint loc;
    loc = glGetUniformLocation(program, name);
    if (loc == -1)
        printf("No such uniform named \"%s\"\n", name);
    
    printOpenGLError();  // Check for OpenGL errors
    return loc;
}

static void reshape(int w, int h) {
    width  = w;
    height = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.5, 30.0);
    glMatrixMode(GL_MODELVIEW);
    
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

 // the display function
void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);  //camera positioning
    
    //This stuff rotates the light source
    LightLocation[0] = 10.0*cos(light_angle);
    LightLocation[1] = 0.0;
    LightLocation[2] = 10.0*sin(light_angle);
    glLightfv(GL_LIGHT0, GL_POSITION, LightLocation);
    
    glutSolidTeapot(1.0);  //draw the teapot
    
    glutSwapBuffers();
    glFlush();
}

int InstallShaders(const GLchar *VertexShaderFile, const GLchar *FragmentShaderFile) {
    GLuint brickVS, brickFS;  // handles to objects
    GLint  vertCompiled, fragCompiled;    // status values
    GLint  linked;
    
    char* vSource = readShaderSource(VertexShaderFile);
    const char **tempv = (const char**)&vSource;
    char* fSource = readShaderSource(FragmentShaderFile);
    const char **tempf = (const char **)&fSource;
    
    // Create a vertex shader object and a fragment shader object
    brickVS = glCreateShader(GL_VERTEX_SHADER);
    brickFS = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Load source code strings into shaders
    
    glShaderSource(brickVS, 1, tempv, NULL);
    glShaderSource(brickFS, 1, tempf, NULL);
    
    // Compile the brick vertex shader and print out
    // the compiler log file.
    
    glCompileShader(brickVS);
    printOpenGLError();  // Check for OpenGL errors
    glGetShaderiv(brickVS, GL_COMPILE_STATUS, &vertCompiled);
    printShaderInfoLog(brickVS);
    
    // Compile the brick fragment shader and print out
    // the compiler log file.
    
    glCompileShader(brickFS);
    printOpenGLError();  // Check for OpenGL errors
    glGetShaderiv(brickFS, GL_COMPILE_STATUS, &fragCompiled);
    printShaderInfoLog(brickFS);
    
    if (!vertCompiled || !fragCompiled) //If either does not compile
    {
        cout<<"shaders are not compiled"<<endl;
        return 0;
    }
    
    // Create a program object and attach the two compiled shaders
    
    PerPixelShader = glCreateProgram();
    
    glAttachShader(PerPixelShader, brickVS);
    glAttachShader(PerPixelShader, brickFS);
    
    // Link the program object and print out the info log
    
    glLinkProgram(PerPixelShader);
    printOpenGLError();  // Check for OpenGL errors
    glGetProgramiv(PerPixelShader, GL_LINK_STATUS, &linked);
    printProgramInfoLog(PerPixelShader);
    cout<<"linked = "<<linked<<endl;
    if (!linked)
        //return 0;
        cout<<"linked = "<<linked<<endl;
    // Install program object as part of current state
    //I took this out so that I could toggle the program used
    //glUseProgram(PerPixelShader);
    
    return 1;
}

void RotateLight(void)  // Idle function to imcrement the angle of the light source
{
    light_angle += 0.01;
    if (light_angle >= 2.0*PI) light_angle =0.0;
    glutPostRedisplay();
}

void SetShader(int key, int x, int y)  //Selects whether to use shaders or built in functionality
{
    if (key == GLUT_KEY_F1) glUseProgram(PerPixelShader);  // Use the per pixel shader
    if (key == GLUT_KEY_F2) glUseProgram(0);  // Use the built-in functionality
    
}
//=================================================================
//Main Program
//=================================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Procedural Brick Texture");
    
    
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(RotateLight);
    glutSpecialFunc(SetShader);
    
    InstallShaders("pointlight.vert", "pointlight.frag");
    glEnable(GL_DEPTH_TEST);
    
    //Set up lighting stuff
    float LightLocation[4] = {0.0, 0.0,10.0,1.0};
    float LightSpecular[4] = {1.0,1.0,1.0,1.0};
    float LightDiffuse[4] = {1.0,1.0,1.0,1.0};
    float DiffuseMaterial[4] = {0.1,0.1,0.1,1.0};
    float SpecularMaterial[4]= {0.6, 0.6,0.6,1.0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, LightLocation);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
    
    glutMainLoop();
    return 0;
}