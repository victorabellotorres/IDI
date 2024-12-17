#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  angulo = parpella = posicioXcocodril = 0;
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTransformMoureCocodril(posicioXcocodril);

  pintaCap();
  pintaUll(parpella);

  pintaMandibula(false, 0.0);// Mandíbula inferior
  pintaMandibula(true, glm::radians(angulo)); // Mandíbula superior
      
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::pintaUll(float percentatgeParpella){
  // Pintem l'ull
  glBindVertexArray(VAO_Quadrat_blanc);
  modelTransformQuadrat(glm::vec3(-0.15, 0.25, 0.0), glm::vec3(0.2));
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Pintem parpella
  glBindVertexArray(VAO_Quadrat_verd);
  modelTransformQuadrat(glm::vec3(-0.15, 0.25, 0.0), glm::vec3(0.2, 0.2*percentatgeParpella, 0.2));
  glDrawArrays(GL_TRIANGLES, 0, 6);

}	

void MyGLWidget::pintaCap(){
  // Pintem el cap
  glBindVertexArray(VAO_Quadrat_verd);
  modelTransformQuadrat(glm::vec3(0.0, 0.4, 0.0), glm::vec3(0.8));
  glDrawArrays(GL_TRIANGLES, 0, 6);    	
}	

void MyGLWidget::pintaMandibula(bool up, double angleApertura){
  glBindVertexArray(VAO_Mandibula);
  modelTransformMandibula(up, angleApertura);
  glDrawArrays(GL_TRIANGLES, 0, 18);
}

void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMandibula(bool up, double angleApertura)
{
  float escala = 0.6;
  glm::mat4 TG(1.0f);
  if (up == true) { // Mandíbula superior
      // abrir la mandíbula
      float aux_tx = escala*(1.0/(4*2)); // distancia x de medio diente
      TG = glm::translate(TG, glm::vec3(-aux_tx, 0.0, 0.0));
      TG = glm::rotate(TG, float(angleApertura), glm::vec3(0.0, 0.0, 1.0));
      TG = glm::translate(TG, glm::vec3(+aux_tx, 0.0, 0.0));

      //colocar la mandíbula cerrada en su sitio
      aux_tx = escala * (1/2.0 - 1.0/(4*2)); //4 s el numero de dientes y lo multiplico por 2 porque se tiene que desplazar medio diente
      TG = glm::translate(TG, glm::vec3(aux_tx, 0.0, 0.0)); // ponerlo en la posición con la boca cerrada
      TG = glm::rotate(TG, float(M_PI), glm::vec3(0.0, 0.0, 1.0)); // rotación 180 grados
      TG = glm::translate(TG, glm::vec3(-(1*escala/2), -(0.2*escala), 0.0)); // se coloca con el centro de la caja contenedora de la mandíbula en la posición (0,0,0)

  } else { // Mandíbula inferior
    TG = glm::translate(TG, glm::vec3(0.0, -(0.2*2*escala), 0.0)); // puntes de dent = 0 (0.2 es la altura de la mandibula/2, 0.6 la escala que se aplica a la mandíbula)
  }
  TG = glm::scale(TG, glm::vec3(escala));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMoureCocodril(float moureXcocodril)
{ // utiliza otra TG para mover el cocodrilo (ya que la TGpos es igual para todos los VAO'S)
  glm::mat4 TGpos(1.0f);
  TGpos = glm::translate(TGpos, glm::vec3(moureXcocodril, 0.0, 0.0));
  glUniformMatrix4fv(TGposLoc, 1, GL_FALSE, &TGpos[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: 
      if (angulo < 45) angulo += 5;
    	break;
    case Qt::Key_Down: 
      if (angulo > 0) angulo -= 5;
    	break;
    case Qt::Key_Right: 
      posicioXcocodril += 0.02;
    	break;
    case Qt::Key_Left: 
      posicioXcocodril -= 0.02;
    	break;
    case Qt::Key_W:
      if (parpella < 1) parpella += 0.1;
    	break;
    case Qt::Key_S: 
      if (parpella > 0) parpella -= 0.1;
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers (){
    creaBuffersMandibula();
    VAO_Quadrat_verd = creaBuffersQuadrat(verd);
    VAO_Quadrat_blanc = creaBuffersQuadrat(blanc);
}

GLuint MyGLWidget::creaBuffersQuadrat (glm::vec3 color)
{
  GLuint VAOId;

  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  Vertices[0] = glm::vec3(-1.0, 0.0, 0);
  Vertices[1] = glm::vec3( 0.0, -1.0, 0);
  Vertices[2] = glm::vec3( 0.0, 0.0, 0);
  Vertices[3] = glm::vec3( 0.0, -1.0, 0);
  Vertices[4] = glm::vec3( -1.0, 0.0, 0);
  Vertices[5] = glm::vec3( -1.0, -1.0, 0);
  
  glm::vec3 Colors[6];
  for(int i=0;i<6;i++) {
      Colors[i] = color;//
  }

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOId);
  glBindVertexArray(VAOId);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  // retornem l'identificador de VAO creat
  return VAOId;
}

void MyGLWidget::creaBuffersMandibula()
{
  int numDents = 4;
  float w = 1;
  float h = 0.2;
  float toothWidth = w / numDents;
  int numVertexs = (numDents+2) * 3;
  glm::vec3 Colors[numVertexs];
  glm::vec3 Vertices[numVertexs];
  glm::vec3 black(0,0,0);
  int i=0;
  Vertices[i++] = glm::vec3(0.0, 0.0, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w, h, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  float x=0;
  for(int d=0;  d<numDents; d++, x+=toothWidth) {
      Vertices[i++] = glm::vec3(x, h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth/2, 2*h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth, h, 0.0);
  }

  for (int i = 0; i < 6;i++) Colors[i] = verd; // Base de la mandíbula
  for(int i=6;i<numVertexs;i++) {
    if (Vertices[i][1] == 2*h) Colors[i] = blanc_dent; // Coloreamos las puntas de los dientes mirando a que altura([i][1]) esta el vértice.
    else Colors[i]=blanc;
  }

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_Mandibula);
  glBindVertexArray(VAO_Mandibula);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificadors per als atributs “vertex” i "color" del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG");
  TGposLoc = glGetUniformLocation(program->programId(), "TGpos");
}
