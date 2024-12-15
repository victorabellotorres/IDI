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

const glm::vec3 MyGLWidget::colores[4] = {
  glm::vec3(0.4, 0.0, 0.0), // rojo
  glm::vec3(0.0, 0.4, 0.0), // verde
  glm::vec3(0.0, 0.0, 0.4), // azul
  glm::vec3(0.4, 0.4, 0.0)  // amarillo
};

void MyGLWidget::initializeGL() {
  LL4GLWidget::initializeGL();

  connect (&timer, SIGNAL (timeout()), this, SLOT (modoDisco ())); // conectar el timeout() del timer a la funcion animar() de public slot de la clase this

  indexLlum = 0;

  carregaLocsFocus();
  iniFocus();

  girBailarins = girBola = 0;
}

void MyGLWidget::carregaLocsFocus() {
  posFocusBlancLoc = glGetUniformLocation (program->programId(), "posFocusBlanc");
  posFocusDiscoLoc[0] = glGetUniformLocation (program->programId(), "posFocusDisco0");
  posFocusDiscoLoc[1] = glGetUniformLocation (program->programId(), "posFocusDisco1");
  posFocusDiscoLoc[2] = glGetUniformLocation (program->programId(), "posFocusDisco2");
  posFocusDiscoLoc[3] = glGetUniformLocation (program->programId(), "posFocusDisco3");

  colorFocusBlancLoc = glGetUniformLocation (program->programId(), "colorFocusBlanc");
  colorFocusDiscoLoc[0] = glGetUniformLocation (program->programId(), "colorFocusDisco0");
  colorFocusDiscoLoc[1] = glGetUniformLocation (program->programId(), "colorFocusDisco1");
  colorFocusDiscoLoc[2] = glGetUniformLocation (program->programId(), "colorFocusDisco2");
  colorFocusDiscoLoc[3] = glGetUniformLocation (program->programId(), "colorFocusDisco3");

}

void MyGLWidget::iniFocus() {
  posFocusBlanc = glm::vec3(glm::vec4(5,10,5,1));
  // los demas se inicializan en el modelTransformDiscoBall para poder usar su TG

  glUniform3fv(posFocusBlancLoc, 1, &posFocusBlanc[0]);

  colorFocusBlanc = glm::vec3(0.4, 0.4, 0.4);

  for (int i = 0; i < 4; ++i) {
    colorFocusDisco[i] = colores[i];
  }

  glUniform3fv(colorFocusBlancLoc, 1, &colorFocusBlanc[0]);

  for (int i = 0; i < 4; ++i) {
    glUniform3fv(colorFocusDiscoLoc[i], 1, &colorFocusDisco[i][0]);
  }
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA 1 (esquerra)
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // FANTASMA 2 (dreta)
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformMorty ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, glm::radians(girBailarins), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, glm::radians(girBailarins), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformDiscoBall ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::rotate(TG, glm::radians(girBola), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

  glm::vec3 posFocusDisco[4] = {
    glm::vec3(TG*glm::vec4(1.2,0,0,1)),
    glm::vec3(TG*glm::vec4(-1.2,0,0,1)),
    glm::vec3(TG*glm::vec4(0,0,1.2,1)),
    glm::vec3(TG*glm::vec4(0,0,-1.2,1))
  };

  for (int i = 0; i < 4; ++i) {
    glUniform3fv(posFocusDiscoLoc[i], 1, &posFocusDisco[i][0]);
  }
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
  case Qt::Key_A: {
      girBailarins += 45;
    break;
	}
  case Qt::Key_D: {
      girBailarins -= 45;
    break;
	}
  case Qt::Key_E: {
      if (colorFocusBlanc == glm::vec3(0, 0, 0))  colorFocusBlanc = glm::vec3(0.4, 0.4, 0.4);
      else  colorFocusBlanc = glm::vec3(0, 0, 0);

      glUniform3fv(colorFocusBlancLoc, 1, &colorFocusBlanc[0]);
    break;
	}
  case Qt::Key_B: {
    if (colorFocusDisco[indexLlum] == glm::vec3(0, 0, 0)) colorFocusDisco[indexLlum] = colores[indexLlum]; 
    else  colorFocusDisco[indexLlum] = glm::vec3(0, 0, 0);

    glUniform3fv(colorFocusDiscoLoc[indexLlum], 1, &colorFocusDisco[indexLlum][0]);
    indexLlum = (indexLlum + 1) % 4;
    break;
	}	
  case Qt::Key_R: {
      girBola += 5;
    break;
	}
  case Qt::Key_S: {
      // encendre i apagar el timer 
      if (timer.isActive()) timer.stop();
      else timer.start(100);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}


// SLOTS

void MyGLWidget::modoDisco() {
  makeCurrent();
  girBola += 5;
  if (colorFocusDisco[indexLlum] == glm::vec3(0, 0, 0)) colorFocusDisco[indexLlum] = colores[indexLlum]; 
  else  colorFocusDisco[indexLlum] = glm::vec3(0, 0, 0);

  glUniform3fv(colorFocusDiscoLoc[indexLlum], 1, &colorFocusDisco[indexLlum][0]);
  indexLlum = (indexLlum + 1) % 4;
  update();
}
