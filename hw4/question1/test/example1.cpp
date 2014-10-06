#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include <iostream>
#include <stdio.h>

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile) {
    FILE* fp = fopen(shaderFile, "r");
    
    if ( fp == NULL ) { return NULL; }
    
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    
    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);
    
    buf[size] = '\0';
    fclose(fp);
    
    return buf;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char* vShaderFile, const char* fShaderFile) {
    struct Shader {
        const char*  filename;
        GLenum       type;
        GLchar*      source;
    }  shaders[2] = {
        { vShaderFile, GL_VERTEX_SHADER, NULL },
        { fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };
    
    GLuint program = glCreateProgram();
    
    for ( int i = 0; i < 2; ++i ) {
        Shader& s = shaders[i];
        s.source = readShaderSource( s.filename );
        if ( shaders[i].source == NULL ) {
            std::cerr << "Failed to read " << s.filename << std::endl;
            exit( EXIT_FAILURE );
        }
        
        GLuint shader = glCreateShader( s.type );
        glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
        glCompileShader( shader );
        
        GLint  compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            std::cerr << s.filename << " failed to compile:" << std::endl;
            GLint  logSize;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            char* logMsg = new char[logSize];
            glGetShaderInfoLog( shader, logSize, NULL, logMsg );
            std::cerr << logMsg << std::endl;
            delete [] logMsg;
            
            exit( EXIT_FAILURE );
        }
        
        delete [] s.source;
        
        glAttachShader( program, shader );
    }
    
    /* link  and error check */
    glLinkProgram(program);
    
    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
        std::cerr << "Shader program failed to link" << std::endl;
        GLint  logSize;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog( program, logSize, NULL, logMsg );
        std::cerr << logMsg << std::endl;
        delete [] logMsg;
        
        exit( EXIT_FAILURE );
    }
    
    glUseProgram(program);
    
    return program;
}

void display( void ) {
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glFlush();
}

int main( int argc, char **argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Sample" );
    
    // Create Vertex Array Object
    GLuint vao;
//    glGenVertexArraysAPPLE(1, &vao);
    //glBindVertexArrayAPPLE(vao);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    /*GLuint vbo;
    glGenBuffers(1, &vbo);
    
    GLfloat vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    
    GLuint elements[] = {0, 1, 2, 2, 3, 0};
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    // Load shaders and use the resulting shader program
    GLuint shaderProgram = InitShader( "c2_exercise_1.vert", "c2_exercise_2.frag" );
    glUseProgram( shaderProgram );
    
    // Initialize the vertex position attribute from the vertex shader
    GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    
    GLint colAttrib = glGetAttribLocation(shaderProgram, "vColor");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));*/
//    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
    
  //  glutDisplayFunc( display );
    //glutMainLoop();
    return 0;
}
