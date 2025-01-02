#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public slots:
    void changeCub(int i);
    void changeCamara(bool b);

  signals:
    void setInt(int n);
    void setOrtogonal();
    void setPerspectiva();

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio (); 
    virtual void projectTransform ();
    virtual void viewTransform ();
 
    virtual void initializeGL ( );

    void reset();

  private:
    int printOglError(const char file[], int line, const char func[]);

    const float angleEntreCubs = (M_PI*2)/3;
    int cubAssociatPatricio;

    int rotarCubs; // un incremento en 1 rotaria la posición antiorariamente(regla de la mano derecha sobre la Y), y un decremento lo rotaria en sentido antihorario.

    enum Elements {Patricio, Cubs};
    Elements quePintar;
};
