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

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up:
      displacementZ -= 0.1;
      break;
    case Qt::Key_Down:
      displacementZ += 0.1;
      break;
    case Qt::Key_Left:
      displacementX -= 0.1;
      break;
    case Qt::Key_Right:
      displacementX += 0.1;
      break;
    case Qt::Key_1: // apagar encender SCA
      colorFocusSCA = (colorFocusSCA == glm::vec3(1.0, 1.0, 0.0)) ? glm::vec3(0.0, 0.0, 0.0) : glm::vec3(1.0, 1.0, 0.0);
      glUniform3fv(colorFocusLocSCA, 1, &colorFocusSCA[0]);
      break;
    case Qt::Key_2: // apagar encender SCO
      colorFocusSCO = (colorFocusSCO == glm::vec3(0.8, 0.8, 0.8)) ? glm::vec3(0.0, 0.0, 0.0) : glm::vec3(0.8, 0.8, 0.8);
      glUniform3fv(colorFocusLocSCO, 1, &colorFocusSCO[0]);
      break;
      case Qt::Key_B: // activar/desactivar back face culling
        if (glIsEnabled(GL_CULL_FACE)) {
          glDisable(GL_CULL_FACE);
        } 
        else {
          glEnable(GL_CULL_FACE);       
        }
        break;
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
  BL3GLWidget::initializeGL();

  carregaShadersFocus();

  displacementX = 0.0;
  displacementZ = 0.0;

  focus = focusFix;
  posFocusSCA = glm::vec3(View*glm::vec4(0.0+displacementX, 0.5 ,0.0+displacementZ, 1.0));
  posFocusSCO = glm::vec3(0.0, 0.0, 0.0);
  colorFocusSCA = glm::vec3(1.0, 1.0, 0.0);
  colorFocusSCO = glm::vec3(0.8, 0.8, 0.8);

  glUniform3fv(posFocusLocSCA, 1, &posFocusSCA[0]);
  glUniform3fv(posFocusLocSCO, 1, &posFocusSCO[0]);
  glUniform3fv(colorFocusLocSCA, 1, &colorFocusSCA[0]);
  glUniform3fv(colorFocusLocSCO, 1, &colorFocusSCO[0]);

}

void MyGLWidget::paintGL() {
  //actualizar focus sca
  posFocusSCA = glm::vec3(View*glm::vec4(0.0+displacementX, 0.5 ,0.0+displacementZ, 1.0));// esto estaria mejor si se multiplicara por la matriz en el fragment pero me da pereza hacerlo :D
  glUniform3fv(posFocusLocSCA, 1, &posFocusSCA[0]);

  BL3GLWidget::paintGL();
}

void MyGLWidget::carregaShadersFocus() {

  posFocusLocSCA = glGetUniformLocation(program->programId(), "posFocusSCA");
  posFocusLocSCO = glGetUniformLocation(program->programId(), "posFocusSCO");

  colorFocusLocSCA = glGetUniformLocation(program->programId(), "colorFocusSCA");
  colorFocusLocSCO = glGetUniformLocation(program->programId(), "colorFocusSCO");
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.0,0.0,0.3);
  diff = glm::vec3(0.3, 0.3, 1.0);
  spec = glm::vec3(0.8, 0.8, 1.0);
  shin = 100;
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::translate(glm::mat4(1.f), glm::vec3(displacementX, 0, displacementZ));
  TG = glm::scale(TG, glm::vec3(escala*0.15, escala*0.15, escala*0.15));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
