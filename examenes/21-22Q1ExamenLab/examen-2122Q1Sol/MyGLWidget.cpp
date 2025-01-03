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

void MyGLWidget::iniEscena ()
{
  ExamGLWidget::iniEscena();
    // Paràmetres de l'escena 
  glm::vec3 Pmin = glm::vec3(0,0,0);
  glm::vec3 Pmax = glm::vec3(30,2.5,24);

  centreEsc = (Pmin+Pmax) / 2.f;
  radiEsc = (glm::distance(Pmin,Pmax)) / 2;
  dist = 2*radiEsc;

  posFocusAvio = glm::vec3((capsaAvioMax.x+capsaAvioMin.x)/2, (capsaAvioMax.y+capsaAvioMin.y)/2, capsaAvioMin.z);
  focusAvioActivat = false;

  rotacioAvio = desplYAvio = 0;
}

void MyGLWidget::iniMaterialTerra()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.0,0.1,0.1);
  diff = glm::vec3(0.0,0.7,0.7);
  spec = glm::vec3(0,1,1);
  shin = 150;
}

void MyGLWidget::enviaPosFocus()
{
  if (focusAvioActivat) posFoc = glm::vec3(View*TGAvio*glm::vec4(posFocusAvio, 1));
  else posFoc = glm::vec3(0, 0, 0);
  glUniform3fv (posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  angleY = -0.5;
  angleX = 0.5;
  camera2 = false;
  ra = float(width())/height();
  fov = 2*glm::asin(radiEsc/(dist));
  zn = dist - radiEsc;
  zf = dist + radiEsc;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(15, 0, 12));
  TG = glm::scale(TG, glm::vec3 (2*escalaPat, 2*escalaPat, 2*escalaPat));
  TG = glm::rotate(TG, glm::radians(-90.f), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAvio ()
{
  TGAvio = glm::mat4(1.f);
  TGAvio = glm::translate(TGAvio, glm::vec3 (25, desplYAvio, 12));

  //transformacions per rotar lavio:
  TGAvio = glm::translate(TGAvio, glm::vec3(-10, 0, 0));
  TGAvio = glm::rotate(TGAvio, float(rotacioAvio*-(2*M_PI/32)), glm::vec3(0, 1, 0)); // rotacioAvio és un valor entre 0 i 31.
  TGAvio = glm::translate(TGAvio, glm::vec3(10, 0, 0));

  TGAvio = glm::scale(TGAvio, glm::vec3 (1.5*escalaAvio, 1.5*escalaAvio, 1.5*escalaAvio));
  TGAvio = glm::translate(TGAvio, -centreBaseAvio);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGAvio[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camera2) {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -dist));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    //lookAt(OBS, VRP, UP)
    View = glm::lookAt(glm::vec3(15, 3, 12), glm::vec3(0, 3, 12), glm::vec3(0, 1, 0));

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camera2) {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else
  {
    glm::mat4 Proj;
    Proj = glm::perspective(float(M_PI/2), ra, 1.f, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      if (desplYAvio < 5) ++desplYAvio;
      emit setAltura(desplYAvio);
    break;
	}
  case Qt::Key_Down: {
      if (desplYAvio > 0) --desplYAvio;
      emit setAltura(desplYAvio);
    break;
	}
  case Qt::Key_Right: {
      rotacioAvio = (rotacioAvio + 1)%32;
    break;
	}
  case Qt::Key_F: {
      focusAvioActivat = !focusAvioActivat;
      enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
      camera2 = !camera2;
      if (camera2) emit setCameraPatricio();
      else emit setCamera3aPers();
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_R: {
      iniCamera();
      focusAvioActivat = false;
      rotacioAvio = desplYAvio = 0;
      enviaPosFocus();
      emit setAltura(desplYAvio);
      emit setCamera3aPers();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camera2)
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

void MyGLWidget::changeCamera(bool es3aPers) {
  makeCurrent();
  if (es3aPers) camera2 = false;
  else camera2 = true;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::changeAlturaAvio(int altura) {
  makeCurrent();
  desplYAvio = altura;
  update();
}


