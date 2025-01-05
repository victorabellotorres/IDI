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

MyGLWidget::MyGLWidget(QWidget *parent) : ExamGLWidget(parent) {

		
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::carregaShaders() {
  ExamGLWidget::carregaShaders();
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");

}

void MyGLWidget::resizeGL(int width, int heigth) {
  ExamGLWidget::resizeGL(width, heigth);

  if (!camara2) {
    if (ra <= 1) fov = 2*atan(tan((fovOptim/2)/ra));
  }
}

void MyGLWidget::iniEscena ()
{
  creaBuffersModels();

  // inicialitzem tots els possibles objectes (blocs de Lego) com a "buits" --> no es pinten ni tenen valors guardats més que els per defecte
  for (int i = 0; i < NUM_BRICKS; i++)
  {
    pintarBricks[i] = false;  	
    brickModelIndex[i] = 1;
    brickTGs[i] = glm::mat4(1);
    brickColors[i] = glm::vec3(1,1,1);
  }
  
  // inicialitzem l'objecte actual (el bloc de Lego) a editar
  currentBrickObjectIndex = 0;
  currentBrickModelIndex = 1;

  // inicialitzem el color actual
  currentColor = white;	

  // Inicialitzem posicio bloc actual
  currentBrickPos = glm::vec3(0, 0, 0);

  rotacio = 0;

  // Inicialitzem els paràmetres de l'escena amb valors arbitraris
  centreEsc = glm::vec3 (0, 4.375, 0);
  radiEsc = sqrt(20*20 + 5.375*5.375 + 20*20);
  dist = 1.5*radiEsc;

  enviaPosFocus();
}

void MyGLWidget::iniCamera ()
{
  camara2 = false;
    // Inicialitzem els paràmetres de càmera amb valors arbitraris
  angleY = glm::radians(15.f);
  angleX = glm::radians(15.f);
  ra = float(width())/height();
  fov = fovOptim = glm::asin(radiEsc/dist);
  zn = dist - radiEsc;
  zf = dist + radiEsc;

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

  
  // Pintem el terra = GROUND_BRICKS
  glUniform3fv(colorLoc,1,&red[0]);
  glBindVertexArray (VAO_models[GROUND_BRICKS]);
  modelTransformGround();
  glDrawArrays(GL_TRIANGLES, 0, models[GROUND_BRICKS].faces().size()*3);
   
  // Pintem el bloc de Lego actual (currentBrick) del color actual
  glUniform3fv(colorLoc,1,&currentColor[0]);
  glBindVertexArray (VAO_models[currentBrickModelIndex]);
  modelTransformCurrentBrick();
  glDrawArrays(GL_TRIANGLES, 0, models[currentBrickModelIndex].faces().size()*3); 

  // bloc de Lego actual en mode GL_LINES
  glUniform3fv(colorLoc,1,&black[0]);
  glBindVertexArray (VAO_models[currentBrickModelIndex]);
  modelTransformCurrentBrick();
  glDrawArrays(GL_LINES, 0, models[currentBrickModelIndex].faces().size()*3); 
  
  //pintar bloques guardados
  int i = 0;
  while (pintarBricks[i]) {
     glUniform3fv(colorLoc,1,&brickColors[i][0]);
    glBindVertexArray (VAO_models[brickModelIndex[i]]);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &brickTGs[i][0][0]);
    glDrawArrays(GL_TRIANGLES, 0, models[brickModelIndex[i]].faces().size()*3); 
    ++i;
  }

  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCurrentBrick()
{
  currentTG = glm::mat4(1.f);

  //if bricks oriented in the x axis
  if (rotacio%180 == 0) {
    if (currentBrickModelIndex == 1) currentTG = glm::translate(currentTG, glm::vec3(0.5, 0, 0));

    if (currentBrickModelIndex == 1 && currentBrickPos.x > 9) currentBrickPos.x = 9;
    else if (currentBrickModelIndex == 2) {
      if (currentBrickPos.x > 9) currentBrickPos.x = 9;
      else if (currentBrickPos.x < -9) currentBrickPos.x = -9;
    }
    else if (currentBrickModelIndex == 3) {
      if (currentBrickPos.x > 8) currentBrickPos.x = 8;
      else if (currentBrickPos.x < -8) currentBrickPos.x = -8;
    }
    if (currentBrickPos.z > 9) currentBrickPos.z = 9;
    if (currentBrickPos.z < -9) currentBrickPos.z = -9;
  }
  else {
    if (currentBrickModelIndex == 1) currentTG = glm::translate(currentTG, glm::vec3(0, 0, 0.5));

    if (currentBrickModelIndex == 1 && currentBrickPos.z > 9) currentBrickPos.z = 9;
    else if (currentBrickModelIndex == 2) {
      if (currentBrickPos.z > 9) currentBrickPos.z = 9;
      else if (currentBrickPos.z < -9) currentBrickPos.z = -9;
    }
    else if (currentBrickModelIndex == 3) {
      if (currentBrickPos.z > 8) currentBrickPos.z = 8;
      else if (currentBrickPos.z < -8) currentBrickPos.z = -8;
    }
    if (currentBrickPos.x > 9) currentBrickPos.x = 9;
    if (currentBrickPos.x < -9) currentBrickPos.x = -9;
  }
  //moure el brick amb tecles
  currentTG = glm::translate(currentTG, currentBrickPos);

  currentTG = glm::rotate(currentTG, glm::radians(float(rotacio)), glm::vec3(0, 1, 0)); 
  currentTG = glm::rotate(currentTG, glm::radians(90.f), glm::vec3(1, 0, 0));
  currentTG = glm::scale(currentTG, glm::vec3 (escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex]));
  currentTG = glm::translate(currentTG, -centreCapsaModels[currentBrickModelIndex]);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &currentTG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -dist));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (!camara2) Proj = glm::perspective(fov, ra, zn, zf);
  else Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      if (currentBrickPos.x > -10) currentBrickPos.x--;
    break;
	}
  case Qt::Key_D: {
      if (currentBrickPos.x < 10) currentBrickPos.x++;
    break;
    }
  case Qt::Key_W: {
      if (currentBrickPos.z < 10) currentBrickPos.z++;
    break;
	}
  case Qt::Key_S: {
      if (currentBrickPos.z > -10) currentBrickPos.z--;
    break;  
	}
  case Qt::Key_Up: {
      if (currentBrickPos.y < 10) currentBrickPos.y++;
    break;
	}
  case Qt::Key_Down: {
      if (currentBrickPos.y > 0) currentBrickPos.y--;
    break;  
	}		
  case Qt::Key_Q: {
      rotacio += 90;
    break;
	}
  case Qt::Key_C: {
    camara2 = !camara2;
    projectTransform();
    break;
	}
  case Qt::Key_Space: {
    colocarBrick();
    break;
    }
  case Qt::Key_R: {
    restart();
    break;
	}
  case Qt::Key_M: { // al prèmer la tecla M canviem el model del bloc que estem editant
    currentBrickModelIndex++;
    if (currentBrickModelIndex == NUM_MODELS) currentBrickModelIndex = 1;
    switch (currentBrickModelIndex)
    {
    case 1:
      emit setSmallRButton();
      break;
    case 2:
      emit setMiddleRButton();
      break;
    case 3: 
      emit setBigRButton();
      break;
    }
    break;
    case Qt::Key_B: {
    break;
	  }
  }
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació (només en Y)
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::enviaPosFocus() {
  posFocus = glm::vec3(0, 0, 0);
  glUniform3fv(posFocusLoc, 0, &posFocus[0]);
}

void MyGLWidget::colocarBrick() {
  if (currentBrickObjectIndex > (NUM_BRICKS -1)) {
    return;
  }
  int i =currentBrickObjectIndex;
  brickModelIndex[i] = currentBrickModelIndex;
  brickTGs[i] = currentTG;
  brickColors[i] = currentColor;
  pintarBricks[i] = true;
  
  ++currentBrickObjectIndex;
}

void MyGLWidget::restart() {
  
  iniCamera();

  currentBrickModelIndex = 1;

  int i = 0;
  while (pintarBricks[i]) {
    pintarBricks[i] = false;
    ++i;
  }
  currentBrickObjectIndex = 0;

  currentBrickPos = glm::vec3(0, 0, 0);
  rotacio = 0;

  emit setInt(255);
  emit setSmallRButton();

}

//--------------------SLOTS----------------------

void MyGLWidget::setSmallBrick(bool esSmall) {
  makeCurrent();
  if (esSmall) currentBrickModelIndex = 1;
  update();
}
void MyGLWidget::setMiddleBrick(bool esMiddle) {
  makeCurrent();
  if (esMiddle) currentBrickModelIndex = 2;
  update();
}
void MyGLWidget::setBigBrick(bool esBig) {
  makeCurrent();
  if (esBig) currentBrickModelIndex = 3;
  update();
}

void MyGLWidget::setRed(int red) {
  makeCurrent();
  currentColor.x = red/255.f;
  update();  
}
void MyGLWidget::setGreen(int green) { 
  makeCurrent();
  currentColor.y = green/255.f;
  update();
}
void MyGLWidget::setBlue(int blue) {
  makeCurrent();
  currentColor.z = blue/255.f;
  update();
}


