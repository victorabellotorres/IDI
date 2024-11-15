#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

//TODO: resize y tecla R

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

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

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL ()
{
    LL2GLWidget::initializeGL();
    connect (&timer, SIGNAL (timeout()), this, SLOT (mourePilota ())); // conectar el timeout() del timer a la funcion animar() de public slot de la clase this
    mostrarPilota = true;
    modoCamara = Perspectiva;
    left = bottom = -radiEscena;
    right = top = radiEscena;
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  viewTransform ();
  projectTransform();

  // Patricio
  // glBindVertexArray (VAO_models[PATRICIO]);
  // modelTransformPatricio (glm::vec3(0,0,0));
  // glDrawArrays(GL_TRIANGLES, 0, models[PATRICIO].faces().size()*3);

  // LEGO
  glBindVertexArray (VAO_models[LEGO]);
  modelTransformLego (posPorter);
  glDrawArrays(GL_TRIANGLES, 0, models[LEGO].faces().size()*3);

  // Pilota
  if (mostrarPilota) {
    glBindVertexArray (VAO_models[PILOTA]);
    modelTransformPilota ();
    glDrawArrays(GL_TRIANGLES, 0, models[PILOTA].faces().size()*3);
  }
  
  // Pared 1
  glBindVertexArray (VAO_Cub);
  modelTransformCub (glm::vec3(0,0,-7.9), glm::vec3(24,2,0.2));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Pared 2
  glBindVertexArray (VAO_Cub);
  modelTransformCub (glm::vec3(0,0,7.9), glm::vec3(24,2,0.2));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Pared 3
  glBindVertexArray (VAO_Cub);
  modelTransformCub (glm::vec3(-11.9,0,0), glm::vec3(0.2,2,16));
  glDrawArrays(GL_TRIANGLES, 0, 36);

// Terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransformLego (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG, glm::radians(-90.f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[LEGO]));
  TG = glm::translate(TG, -centreBaseModels[LEGO]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(24.f/8, 1, 16.f/8));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCub (glm::vec3 pos, glm::vec3 mida)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, mida); // como tamaño original es 1,1,1, no hace falta dividir por el tamaño anterior
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (modoCamara == Perspectiva)
  {
    Proj = glm::perspective (float(2*asin(radiEscena/(2*radiEscena))), ra, (2.f*radiEscena)-radiEscena, (2.f*radiEscena)+radiEscena);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else if (modoCamara == Ortogonal)
  {
    Proj = glm::ortho(left, right, bottom, top, 16-radiEscena, 16+3*radiEscena);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  //View = glm::lookAt (glm::vec3(0,0,2*radiEscena), glm::vec3(0,0,0), glm::vec3(0,1,0));
  if (modoCamara == Perspectiva) {
    View = glm::translate(View, glm::vec3(0,0,-2*radiEscena));
    View = glm::rotate(View, theta, glm::vec3(1,0,0));
    View = glm::rotate(View, -psi, glm::vec3(0,1,0));
    View = glm::translate(View, glm::vec3(0,-2,0));
  }
  else if (modoCamara == Ortogonal) {
    View = lookAt(glm::vec3(0,18,0), glm::vec3(0,2,0), glm::vec3(-1,0,0));
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniEscena()
{
  LL2GLWidget::iniEscena();
  radiEscena = sqrt(24*24 + 16*16 + 4*4)/2;
  psi = 0;
  theta = float(glm::radians(45.f));
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
        if (!timer.isActive()) timer.start(100); // cada 100 milisegundos
      break;
    }
    case Qt::Key_I: { 
          timer.stop();
          dirInicialPilota();
          posPilota = glm::vec3(9.0, 0.0, 0.0);
          mostrarPilota = true;
      break;
        }           
    case Qt::Key_Left: { 
        if (posPorter.z+0.5 <=6) posPorter += glm::vec3(0,0,0.5);
      break;
        }  
    case Qt::Key_Right: { 
        if (posPorter.z-0.5 >= -6) posPorter += glm::vec3(0,0,-0.5);
      break;
        }
    case Qt::Key_C: { 
        if (modoCamara == Perspectiva) modoCamara = Ortogonal;
        else modoCamara = Perspectiva;
      break;
        }           
    case Qt::Key_R: { 
      break;
        }  
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE && modoCamara == Perspectiva)
  {
          // currentx - oldx
    psi -= (e->x() - xClick) * factorAngleY;
          // currenty - oldy
    theta += (e->y() - yClick) * factorAngleX;
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::rebotaParets()
{
  if (posPilota.x-1 <= -11.8) dirPilota.x = dirPilota.x*-1;
  
  if (posPilota.z+1 >= 7.8 || posPilota.z-1 <= -7.8) dirPilota.z = dirPilota.z*-1;
}

void MyGLWidget::mourePilota()
{
  makeCurrent();

  if (posPilota.x+0.5 >= 12) {
    timer.stop();
    mostrarPilota = false;
  }

  posPilota += glm::vec3(dirPilota);
  if (rebotaPorter()) canviaDireccio();

  rebotaParets();
  update();
}

