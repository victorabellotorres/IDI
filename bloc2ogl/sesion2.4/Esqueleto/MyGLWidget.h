// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include <QMouseEvent>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

//funciones que quieres redefinir del padre
  virtual void creaBuffers ();
  virtual void carregaShaders ();
  virtual void initializeGL ( );
  virtual void modelTransform();
  virtual void paintGL ( );
  virtual void keyPressEvent (QKeyEvent *event);
  virtual void resizeGL (int width, int height);

//funciones creadas en esta clase hijo
  void modelTransformPatricio();
  void modelTransformSuelo();
  void viewTransform();
  void projectTransform();
  void CentroYRadioEsferaContenedora(glm::vec3 Pmax, glm::vec3 Pmin, glm::vec3 &centro, float &radio);
  void centroYAlturaModelo(Model &model, glm::vec3 &centro, float &altura);
  void mousePressEvent(QMouseEvent* e);
  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);




  Model m; // un únic model

  GLuint VAO_Patricio, VAO_Suelo;

  GLuint PMLoc, VMLoc;

  glm::vec3 Pmin, Pmax, centro, centroModelo;

  float angulo, radio, relacionAspecto, distanciaVRPOBS, FOVoptimo, FOV, alturaModelo;

  float left, right, bottom, top;

  float zoom;

  float psi, theta;// Ψ, Θ   (φ esta última sería phi)

  float current_x, current_y, old_x, old_y;

  bool mouseActivo;

  enum ModoProyeccion {
    Perspectiva,
    Ortogonal
  };

  ModoProyeccion modoCamara;

  private:
    int printOglError(const char file[], int line, const char func[]);
};