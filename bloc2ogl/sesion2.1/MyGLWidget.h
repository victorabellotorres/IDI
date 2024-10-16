// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

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


  void viewTransform();
  void projectTransform();

  Model m; // un únic model

  GLuint VAO_Homer;

  GLuint PMLoc, VMLoc;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
