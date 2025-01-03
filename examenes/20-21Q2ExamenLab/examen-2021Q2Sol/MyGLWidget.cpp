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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL() {
  ExamGLWidget::initializeGL();

  anglePatricio = 0;
}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersEsfera();

  // Paràmetres de l'escena - arbitraris
  centreEsc = glm::vec3 (15, 2, 10);
  radiEsc = sqrt(15*15+10*10+2*2);

  enviaPosFocus();
}

void MyGLWidget::enviaPosFocus()
{
  ExamGLWidget::enviaPosFocus();
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.5;
  angleX = -0.5;
  camPlanta = false;
  ra = float(width())/height();
  fov = fovOptimo = 2*glm::asin(radiEsc/(2*radiEsc));
  zn = 2*radiEsc - radiEsc;
  zf = 2*radiEsc + radiEsc;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pintem l'arbre
  pintaArbre (glm::vec3(10,0,10));
  pintaArbre (glm::vec3(20,0,5));
  pintaArbre (glm::vec3(25,0,15));



  glBindVertexArray(0);
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(15, 0, 10));

  //transformacion per moure el patricio al voltant de l'arbre 1 (a la posició 10, 0, 10)
  TG = glm::translate(TG, glm::vec3(-5, 0, 0));
  TG = glm::rotate(TG, float(-anglePatricio*(2*M_PI/31)), glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));

  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camPlanta) {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(1,0,0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);

  }
  else
  {
    // Codi per a la viewMatrix de la Càmera-2
    //lookAt(OBS, VRP, UP)
    View = glm::lookAt(glm::vec3(15, 2*radiEsc, 10), glm::vec3(15, 2, 10), glm::vec3(0, 0, -1));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  if (posFoc != glm::vec3(0, 0, 0)) {
    posFoc = glm::vec3(View*(glm::vec4(15, 10, 10, 1)));
    glUniform3fv (posfocusLoc, 1, &posFoc[0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta) {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else
  {
    // Codi per a la projectMatrix de la Càmera-2
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, zn, zf);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      anglePatricio = (anglePatricio+1)%32;
      emit setInt(anglePatricio);
    break;
	}
  case Qt::Key_F: {
      if (posFoc == glm::vec3(0, 0, 0)) posFoc = glm::vec3(View*glm::vec4(15,10,10,1));
      else posFoc = glm::vec3(0, 0, 0);
      glUniform3fv (posfocusLoc, 1, &posFoc[0]);
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if (camPlanta) emit setOrtogonal();
      else emit setPerspectiva();

      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_R: {
      iniCamera();
      enviaPosFocus();
      anglePatricio = 0;
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
  {
    // Fem la rotació
    angleY -= (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::resizeGL(int w, int h) {
  ExamGLWidget::resizeGL (w, h);

  if (ra >= 1) {

  } else {
    fov = 2*atan(tan(fovOptimo/2)/ra);
  }
  projectTransform();
}

void MyGLWidget::changeCamera(bool esPerspectiva) {
  makeCurrent();
  if (esPerspectiva) camPlanta = false;
  else camPlanta = true;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::rotarPatricio(int num) {
  makeCurrent();
  anglePatricio = num;
  update();
}


