// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

void MyGLWidget::creaBuffers () {
    m.load ("../models/HomerProves.obj");

    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);

    //posicion
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData (GL_ARRAY_BUFFER,
                sizeof(GLfloat) * m.faces ().size () * 3 * 3,
                m.VBO_vertices (), GL_STATIC_DRAW);// posició

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData (GL_ARRAY_BUFFER,
                sizeof(GLfloat) * m.faces ().size () * 3 * 3,
                m.VBO_matdiff (), GL_STATIC_DRAW); // color

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::paintGL ( ) 
{
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);
    
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    modelTransform ();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Homer);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

    glBindVertexArray (0);
}


void MyGLWidget::carregaShaders ()
{
    BL2GLWidget::carregaShaders();
    // aqui modificas como quieras la funcion
    PMLoc = glGetUniformLocation (program->programId(), "PM");
    VMLoc = glGetUniformLocation (program->programId(), "VM");
}

void MyGLWidget::initializeGL ( ) {
    BL2GLWidget::initializeGL();
    // aqui modificas como quieras la funcion
    viewTransform();
    projectTransform();

    glEnable (GL_DEPTH_TEST); // acitvar el Z-Buffer
}

void MyGLWidget::modelTransform () 
{
    BL2GLWidget::modelTransform();
}

void MyGLWidget::viewTransform () {
    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (glm::vec3(0,0,1),
    glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv (VMLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f);
    glUniformMatrix4fv (PMLoc, 1, GL_FALSE, &Proj[0][0]);
}

MyGLWidget::~MyGLWidget() {
}




