#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public slots: // funciónes que se puede usar con signals
    void mourePilota();

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void paintGL();
    virtual void modelTransformTerra();
    virtual void modelTransformCub(glm::vec3 pos, glm::vec3 mida);
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void initializeGL();
    virtual void iniEscena (); 
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void resizeGL (int width, int height);

    void modelTransformLego(glm::vec3 pos); 
    void rebotaParets();
  private:
  
    int printOglError(const char file[], int line, const char func[]);

    float psi, theta; // falta phi

    bool mostrarPilota;
    float left, right, bottom, top;
    float FOVoptimo, FOV;

    enum ModoProyeccion {
      Perspectiva,
      Ortogonal
    };
    ModoProyeccion modoCamara;
   
};
