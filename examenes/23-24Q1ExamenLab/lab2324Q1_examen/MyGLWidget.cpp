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

  // Inicialitzem els paràmetres de l'escena amb valors arbitraris
  centreEsc = glm::vec3 (0, -0.5, 0);
  radiEsc = 18;
  
  rotacio = 0.;
  movX = movY = movZ = 0;

}

void MyGLWidget::iniCamera ()
{
  angleY = float(glm::radians(15.));
  angleX = -float(glm::radians(15.));
  ra = float(width())/height();
  fov = float(M_PI/3.0);
  zn = 1;
  zf = 85;
  l = d = -25.f;
  u = r = 25.f;

  orthoCamera = false;
  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
    glViewport (0, 0, ample, alt);
   // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glm::vec3 red = glm::vec3(0.7,0.,0.);
  // Pintem el terra = GROUND_BRICKS
  glUniform3fv(colorLoc,1,&red[0]);
  glBindVertexArray (VAO_models[GROUND_BRICKS]);
  modelTransformGround();
  glDrawArrays(GL_TRIANGLES, 0, models[GROUND_BRICKS].faces().size()*3);
   
  // Pintem el bloc de Lego actual (currentBrick) del color actual
  for (int i = 0; i <= nbrick; ++i){
      if (nbrick == i){
        glUniform3fv(colorLoc,1,&currentColor[0]);
        glBindVertexArray (VAO_models[currentBrickModelIndex]);
        modelTransformCurrentBrick();
        glDrawArrays(GL_TRIANGLES, 0, models[currentBrickModelIndex].faces().size()*3); 
        glDrawArrays(GL_LINES, 0, models[currentBrickModelIndex].faces().size()*3); 
      }
      else {
          glUniform3fv(colorLoc,1,&brickColors[i][0]);
          glBindVertexArray (VAO_models[brickModelIndex[i]]);
          glUniformMatrix4fv (transLoc, 1, GL_FALSE, &brickTGs[i][0][0]);
          glDrawArrays(GL_TRIANGLES, 0, models[brickModelIndex[i]].faces().size()*3); 
          glDrawArrays(GL_LINES, 0, models[brickModelIndex[i]].faces().size()*3); 
      }
  }
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCurrentBrick()
{
    if (currentBrickModelIndex ==1){
        currentTG = glm::mat4 (1.f);
        if (rotacio == 0. or rotacio == 180.) currentTG = glm::translate(currentTG, glm::vec3(0.5,0.0,0.0));  
        else  currentTG = glm::translate(currentTG, glm::vec3(0.0,0.0,-0.5)); 
        currentTG = glm::translate(currentTG, glm::vec3(movX,0.125+movY,movZ));  
        currentTG = glm::rotate(currentTG, float(glm::radians(rotacio)), glm::vec3(0,1,0));  
        currentTG = glm::rotate(currentTG, float(glm::radians(90.)), glm::vec3(1,0,0));  
        currentTG = glm::scale(currentTG, glm::vec3 (escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex]));
        currentTG = glm::translate(currentTG, -centreCapsaModels[currentBrickModelIndex]);  
        glUniformMatrix4fv (transLoc, 1, GL_FALSE, &currentTG[0][0]);
    }
    else {
      currentTG = glm::mat4 (1.f);
        currentTG = glm::translate(currentTG, glm::vec3(movX,0.125+movY,movZ));  
         currentTG = glm::rotate(currentTG, float(glm::radians(rotacio)), glm::vec3(0,1,0));
        currentTG = glm::rotate(currentTG, float(glm::radians(90.)), glm::vec3(1,0,0));  
        currentTG = glm::scale(currentTG, glm::vec3 (escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex]));
        currentTG = glm::translate(currentTG, -centreCapsaModels[currentBrickModelIndex]);  
        glUniformMatrix4fv (transLoc, 1, GL_FALSE, &currentTG[0][0]);  
    }
}

void MyGLWidget::viewTransform ()
{
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(glm::radians(180.))-angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  if (!orthoCamera)ExamGLWidget::projectTransform();
  else {
      glm::mat4 Proj;  // Matriu de projecció
      Proj = glm::ortho(l,r,d,u, zn, zf);

      glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
      
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      movX -= 1.;
      comprovaMesures();
    break;
	}
  case Qt::Key_D: {
      movX += 1.;
      comprovaMesures();
    break;
    }
  case Qt::Key_W: {
      movZ += 1.;
      comprovaMesures();
    break;
	}
  case Qt::Key_S: {
      movZ -= 1.;
      comprovaMesures();
    break;  
	}
  case Qt::Key_Up: {
      movY += 1.;
      comprovaMesures();
    break;
	}
  case Qt::Key_Down: {
      movY -= 1.;
      comprovaMesures();
    break;  
	}		
  case Qt::Key_Q: {
      rotacio += 90.;
      if (rotacio == 360.) rotacio = 0;
      comprovaMesures();
    break;
	}
  case Qt::Key_C: {
      orthoCamera = !orthoCamera;
      projectTransform();
    break;
	}
  case Qt::Key_Space: {
      if (nbrick < 50){
       brickColors[nbrick] = currentColor;
       brickTGs[nbrick] = currentTG;
       brickModelIndex[nbrick] =currentBrickModelIndex;
       ++nbrick;
      }
      
    break;
    }
  case Qt::Key_M: { // al prèmer la tecla M canviem el model del bloc que estem editant
        currentBrickModelIndex++;
        if (currentBrickModelIndex == NUM_MODELS) currentBrickModelIndex = 1;
        if(currentBrickModelIndex == 1) emit pecaPetitaSignal();
        else if(currentBrickModelIndex == 2) emit pecaMitjanaSignal();
        else emit pecaGranSignal();
        comprovaMesures();
        break;
    }
  case Qt::Key_R: {
      ExamGLWidget::initializeGL ();
      emit pecaPetitaSignal();
      nbrick =0;
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  
  update();
}

void MyGLWidget::comprovaMesures(){
    if (movY > 10.) movY = 10.;
    else if (movY < 0.) movY = 0.;
    if (rotacio == 0. or rotacio == 180.){
        if (currentBrickModelIndex == 1 and movX >9.) movX = 9.;
        if (currentBrickModelIndex == 1 and movX <-10.) movX = -10.;
        else if (currentBrickModelIndex == 2 and movX < -9.) movX = -9.;
        else if (currentBrickModelIndex == 2 and movX > 9.) movX = 9.;
        else if (currentBrickModelIndex == 3 and movX > 8.) movX = 8.;
        else if (currentBrickModelIndex == 3 and movX < -8.) movX = -8.;
        if (movZ > 9.) movZ = 9.;
        else if (movZ < -9.) movZ = -9.;
    }
    else {
        if (currentBrickModelIndex == 1 and movZ < -10.) movZ = -10.;
        else if ((currentBrickModelIndex == 1 or currentBrickModelIndex == 2) and movZ > 9.) movZ = 9.;
        else if (currentBrickModelIndex == 2 and movZ < -9.) movZ = -9.;
        else if (currentBrickModelIndex == 3 and movZ > 8.) movZ = 8.;
        else if (currentBrickModelIndex == 3 and movZ < -8.) movZ = -8.;
        if (movX > 9.) movX = 9.;
        else if (movX < -9.) movX = -9.;
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació (només en Y)
    angleY -= (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}


void MyGLWidget::reiniciaSlot(){
    makeCurrent();
    ExamGLWidget::initializeGL ();
    emit pecaPetitaSignal();
    nbrick = 0;
    update();
}


void MyGLWidget::pecaPetitaSlot(){
    makeCurrent();
    currentBrickModelIndex = 1;
    comprovaMesures();
    update();
}

void MyGLWidget::pecaMitjanaSlot(){
    makeCurrent();
    currentBrickModelIndex = 2;
    comprovaMesures();
    update();
}


void MyGLWidget::pecaGranSlot(){
    makeCurrent();
    currentBrickModelIndex = 3;
    comprovaMesures();
    update();
}

void MyGLWidget::canviaColor(){
    makeCurrent();
    if (currentColor == white)
	    	  currentColor = glm::vec3(0,0,1);
		else 
		  currentColor = white;
	    	
    update();
}


