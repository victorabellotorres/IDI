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

    // HOMER-------------------------------------------
    m.load ("../../models/HomerProves.obj");
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

    
    //Suelo -------------------------------------------------

    glGenVertexArrays(1, &VAO_Suelo);
    glBindVertexArray(VAO_Suelo);

    glm::vec3 VerticesSuelo[6];
    VerticesSuelo[0] = glm::vec3(-2.0, 0.0, -2.0);
    VerticesSuelo[1] = glm::vec3(2.0, 0.0, 2.0);
    VerticesSuelo[2] = glm::vec3(-2.0, 0.0, 2.0);
    VerticesSuelo[3] = glm::vec3(-2.0, 0.0, -2.0);
    VerticesSuelo[4] = glm::vec3(2.0, 0.0, -2.0);
    VerticesSuelo[5] = glm::vec3(2.0, 0.0, 2.0);

    glm::vec3 ColorSuelo[6];
    for (int i = 0; i < 6; ++i) {
        ColorSuelo[i] = glm::vec3(1.0, 0.0, 0.75);
    }

    GLuint VBO_Suelo[2];
    glGenBuffers(2, VBO_Suelo);

    //posicion
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Suelo[0]);
    glBufferData (GL_ARRAY_BUFFER,
                sizeof(VerticesSuelo),
                VerticesSuelo, GL_STATIC_DRAW);// posició
    
    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    //color
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Suelo[1]);
    glBufferData (GL_ARRAY_BUFFER,
                sizeof(ColorSuelo),
                ColorSuelo, GL_STATIC_DRAW); // color// color
    
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

    //PintarHomer-------------------------------------

    // Carreguem la transformació del Homer
    modelTransformHomer ();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Homer);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);


    //PintarSuelo-------------------------------------

    // Carreguem la transformació de model
    modelTransformSuelo();
    
    glBindVertexArray(VAO_Suelo);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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

    glEnable (GL_DEPTH_TEST); // acitvar el Z-Buffer

    angulo = 0;
    //Pmin sacados del tamaño del suelo y la altua de homer que es 2
    Pmin = glm::vec3(-2.0, -1.0, -2.0);
    Pmax = glm::vec3(2.0, 1.0, 2.0);

    //Calcular centro y radio de la esfera contenedora
    CentroYRadioEsferaContenedora(Pmax, Pmin, centro, radio);

    viewTransform();
    projectTransform();

}



void MyGLWidget::modelTransform () 
{
    BL2GLWidget::modelTransform();
}

void MyGLWidget::modelTransformHomer() {
    glm::mat4 TG(1.0f);
    TG = glm::rotate(TG, angulo, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformSuelo() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, -1, 0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform () {

    //vrp = centro
    glm::vec3 obs = centro + glm::vec3(0, 0, 2*radio);

    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (obs, centro, glm::vec3(0,1,0));
    glUniformMatrix4fv (VMLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (float(2*asin(radio/(2*radio))), 1.0f, float(2*radio-radio), float(2*radio+radio));
    glUniformMatrix4fv (PMLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    BL2GLWidget::keyPressEvent(event);
    makeCurrent();
    makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // escalar a més gran
      angulo += M_PI/4;//45 grados
      break;
    }
    default: event->ignore(); break;
  }
  update();

}

void MyGLWidget::CentroYRadioEsferaContenedora(glm::vec3 Pmax, glm::vec3 Pmin, glm::vec3 &centro, float &radio) {
    centro = (Pmax + Pmin) / 2.0f;
    radio = glm::distance(Pmax, centro);
}

MyGLWidget::~MyGLWidget() {
}




