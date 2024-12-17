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

void MyGLWidget::initializeGL(){
  ExamGLWidget::initializeGL();

  // Ya lo hazen en la clase padre
  colFoc = glm::vec3(1, 1, 1);
  enviaColFocus();

  rotarCubs = 0;
  cubAssociatPatricio = 0;
  quePintar = Cubs;
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (quePintar == Patricio) {
    //Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else if (quePintar == Cubs) {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2, angleEntreCubs*(0 + rotarCubs));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub(2.5, angleEntreCubs*(1 + rotarCubs));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub(3, angleEntreCubs*(2 + rotarCubs));
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  glBindVertexArray(0);
}


void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  escala = escala/0.5; //0.5 es la mida del costat del cub
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0, 1,0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  //ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angleEntreCubs*(cubAssociatPatricio + rotarCubs), glm::vec3(0, 1,0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));
  TG = glm::rotate(TG, glm::radians(-90.f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    //lookAt(OBS, VRP, UP)
    View = glm::lookAt(glm::vec3(0, radiEsc, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    // glm::ortho(left, right, bottom, top, zNear, zFar)
    glm::mat4 Proj;
    if (ra > 1) Proj = glm::ortho(-radiEsc*ra, +radiEsc*ra, -radiEsc, +radiEsc, zn-radiEsc, zf + radiEsc);
    else Proj = glm::ortho(-radiEsc, +radiEsc, -radiEsc/ra, +radiEsc/ra, zn-radiEsc, zf + radiEsc);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}



void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      if (quePintar == Cubs) quePintar = Patricio;
      else quePintar = Cubs;
    break;
	}
  case Qt::Key_1: {
      cubAssociatPatricio = 0;
      emit setInt(cubAssociatPatricio+1);
    break;
	}
  case Qt::Key_2: {
      cubAssociatPatricio = 1;
      emit setInt(cubAssociatPatricio+1);
    break;
	}
  case Qt::Key_3: {
      cubAssociatPatricio = 2;
      emit setInt(cubAssociatPatricio+1);
    break;
	}
  case Qt::Key_F: {
      if (colFoc == glm::vec3(1,1,1)) colFoc = glm::vec3(1,1,0);
      else colFoc = glm::vec3(1,1,1);
      enviaColFocus();
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
  case Qt::Key_Right: {
      rotarCubs++;
    break;
	}
  case Qt::Key_Left: {
      rotarCubs--;
    break;
	}
  case Qt::Key_R: {
      reset();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::reset() {
  makeCurrent();
  rotarCubs = 0;
  cubAssociatPatricio = 0;
  quePintar = Cubs;
  colFoc = glm::vec3(1,1,1);
  enviaColFocus();
  angleY = 0.65;
  angleX = -1.2;
  camPlanta = false;
  viewTransform();
  projectTransform();
  emit setInt(1);
  emit setPerspectiva();
  update();
}


//SLOTS

void MyGLWidget::changeCub(int cub) {
  makeCurrent();
  cubAssociatPatricio = cub-1;
  update();
}

void MyGLWidget::changeCamara(bool esPerspectiva) {
  makeCurrent();
  if (esPerspectiva) camPlanta = false;
  else camPlanta = true;
  viewTransform();
  projectTransform();
  update();
}



