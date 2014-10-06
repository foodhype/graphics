#define GLEW_STATIC

// Headers
#include <iostream>
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#define GLSL(src) "#version 130\n" #src

// Shader sources
const GLchar* vertexSource = GLSL(
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main() {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
);

const GLchar* fragmentSource = GLSL(
    in vec3 Color;

    out vec4 outColor;

    void main() {
        outColor = vec4(Color, 1.0);
    }
);


void display(void) {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Exercise Base");
    glClearColor(0.5, 0.5, 1.0, 1.0);
    
    // Initialize GLEW
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
         0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
//    glLinkProgram(shaderProgram);
//    glUseProgram(shaderProgram);


    // Load shaders and use the resulting shader program
    GLuint shaderProgram = InitShader( "c2_exercise_1.vert", "c2_exercise_2.frag" );
    glUseProgram( shaderProgram );



    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));



    // Load shaders and use the resulting shader program
    //GLuint shaderProgram = InitShader( "c2_exercise_1.vert", "c2_exercise_2.frag" );
    //glUseProgram( shaderProgram );

    // Initialize the vertex position attribute from the vertex shader
/*    GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "vColor");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));*/
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background






    glutDisplayFunc(display);    
    glutMainLoop();

    glDeleteProgram(shaderProgram);
    //glDeleteShader(fragmentShader);
    //glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
    return 0;
}
