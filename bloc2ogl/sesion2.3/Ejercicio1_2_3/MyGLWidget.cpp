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
    m.load ("../../models/Patricio.obj");

    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    GLuint VAO_Patricio[2];
    glGenBuffers(2, VAO_Patricio);

    //posicion
    glBindBuffer(GL_ARRAY_BUFFER, VAO_Patricio[0]);
    glBufferData (GL_ARRAY_BUFFER,
                sizeof(GLfloat) * m.faces ().size () * 3 * 3,
                m.VBO_vertices (), GL_STATIC_DRAW);// posició

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VAO_Patricio[1]);
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
    VerticesSuelo[0] = glm::vec3(-2.5, 0.0, -2.5);
    VerticesSuelo[1] = glm::vec3(2.5, 0.0, 2.5);
    VerticesSuelo[2] = glm::vec3(-2.5, 0.0, 2.5);
    VerticesSuelo[3] = glm::vec3(-2.5, 0.0, -2.5);
    VerticesSuelo[4] = glm::vec3(2.5, 0.0, -2.5);
    VerticesSuelo[5] = glm::vec3(2.5, 0.0, 2.5);

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


    viewTransform();
    projectTransform();

    //PintaPatricio-------------------------------------

    // Carreguem la transformació del Patricio
    modelTransformPatricio ();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Patricio);

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

    //Calculo caja contenedora patricio
    centroYAlturaModelo(m, centroModelo, alturaModelo);

    angulo = 0;
    //Pmin sacados del tamaño del suelo y la altura de homer que es 2
    Pmin = glm::vec3(-2.5, 4.0, -2.5);
    Pmax = glm::vec3(2.5, 0.0, 2.5);
    relacionAspecto = 1.0f;

    modoCamara = Perspectiva;

    //Calcular centro y radio de la esfera contenedora
    CentroYRadioEsferaContenedora(Pmax, Pmin, centro, radio);

    distanciaVRPOBS = 1.5*radio;

    psi = theta = 0;
    current_x = current_y = old_x = old_y = 0;
    mouseActivo = false;


    right = top = radio;
    left = bottom = -radio;
    FOVoptimo = FOV = float(2*asin(radio/(distanciaVRPOBS)));

    viewTransform();
    projectTransform();

}

void MyGLWidget::resizeGL (int width, int height) {
    BL2GLWidget::resizeGL(width, height);

    relacionAspecto = float(width) / float(height);
    if (relacionAspecto < 1) {
        //modificar perspectiva
        FOV = 2.0f * atan(tan(FOVoptimo/2.0f) / relacionAspecto);

        //modificar  ortogonal
        right = radio;
        left = -radio;
        top = radio/relacionAspecto;
        bottom = -radio/relacionAspecto;
    } else {
        //modificar perspective 
        FOV = FOVoptimo;

        //modificar  ortogonal
        right = radio*relacionAspecto;
        left = -radio*relacionAspecto;
        top = radio;
        bottom = -radio;
    }

    projectTransform();
}



void MyGLWidget::modelTransform () 
{
    BL2GLWidget::modelTransform();
}

void MyGLWidget::modelTransformPatricio() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 2, 0));
    TG = glm::rotate(TG, angulo, glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(4.0f/alturaModelo, 4.0f/alturaModelo, 4.0f/alturaModelo));
    TG = glm::translate(TG, -centroModelo);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformSuelo() {
    glm::mat4 TG(1.0f);
    TG = glm::scale(TG, glm::vec3(1, 1, 1));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform () {

    //vrp = centro
    //glm::vec3 obs = centro + glm::vec3(0, 0, distanciaVRPOBS);

    //Crear viewMatrix a partir de los angulos de euler
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(0, 0, -distanciaVRPOBS));
    View = glm::rotate(View, theta, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -psi, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centro);
    glUniformMatrix4fv (VMLoc, 1, GL_FALSE, &View[0][0]);

}

void MyGLWidget::projectTransform () {
    glm::mat4 Proj(1.0f);
    if (modoCamara == Perspectiva)
    {
        // glm::perspective (FOV en radians, ra window, znear, zfar)
        Proj = glm::perspective (FOV, relacionAspecto, float(distanciaVRPOBS-radio), float(distanciaVRPOBS+radio));
    } 
    else {
        // glm::ortho (left, right, bottom, top, znear, zfar)
        Proj = glm::ortho(left, right, bottom, top, float(distanciaVRPOBS-radio), float(distanciaVRPOBS+radio));
    }
    glUniformMatrix4fv (PMLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    BL2GLWidget::keyPressEvent(event);
    makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // escalar a més gran
      angulo += M_PI/4;//45 grados
      break;
    case Qt::Key_O: {
        if (modoCamara == Perspectiva) modoCamara = Ortogonal;
        else modoCamara = Perspectiva;
    break;
    }
    }
    default: event->ignore(); break;
  }
  update();

}

void MyGLWidget::mousePressEvent(QMouseEvent* e) {
    makeCurrent();
    if (e->buttons() == Qt::LeftButton) mouseActivo = true;
    old_x = e->x();
    old_y = e->y();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent* e) {
    makeCurrent();
    if (e->buttons() == Qt::LeftButton) mouseActivo = false;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* e) {
    if (mouseActivo) {
        current_x = e->x();
        current_y = e->y();

        float delta_x, delta_y;

        delta_x = current_x - old_x;
        psi += 2*M_PI*delta_x/ample;

        delta_y = old_y - current_y; // estan al reves porque las codenadas de la ventana de qt son diferentes a la del viewport
        theta += 2*M_PI*delta_y/alt;

        old_x = current_x;
        old_y = current_y;

        update();
    }
}



void MyGLWidget::CentroYRadioEsferaContenedora(glm::vec3 Pmax, glm::vec3 Pmin, glm::vec3 &centro, float &radio) {
    centro = (Pmax + Pmin) / 2.0f;
    radio = glm::distance(Pmax, centro);
}

void MyGLWidget::centroYAlturaModelo(Model &model, glm::vec3 &centro, float &altura) {
    glm::vec3 PminAux, PmaxAux;
    PminAux = PmaxAux = glm::vec3(0.0f, 0.0f, 0.0f);
    for (long int i= 0; (long unsigned int)i < model.vertices().size(); i += 3) {
        if (i == 0) 
        {
            PminAux.x = PmaxAux.x = model.vertices()[i];
            PminAux.y = PmaxAux.y = model.vertices()[i+1];
            PminAux.z = PmaxAux.z = model.vertices()[i+2];
        } else 
        {
            if (model.vertices()[i] < PminAux.x) PminAux.x = model.vertices()[i];
            if (model.vertices()[i+1] < PminAux.y) PminAux.y = model.vertices()[i+1];
            if (model.vertices()[i+2] < PminAux.z) PminAux.z = model.vertices()[i+2];

            if (model.vertices()[i] > PmaxAux.x) PmaxAux.x = model.vertices()[i];
            if (model.vertices()[i+1] > PmaxAux.y) PmaxAux.y = model.vertices()[i+1];
            if (model.vertices()[i+2] > PmaxAux.z) PmaxAux.z = model.vertices()[i+2];
        }
    }
    centro = (PmaxAux + PminAux) / 2.0f;
    altura = PmaxAux.y - PminAux.y;
}


MyGLWidget::~MyGLWidget() {
}



