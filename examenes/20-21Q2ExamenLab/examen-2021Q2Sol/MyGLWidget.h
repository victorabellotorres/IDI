#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT
  public slots:
    void changeCamera(bool esPerspectiva);
    void rotarPatricio(int num);

  signals:
    void setPerspectiva();
    void setOrtogonal();
    void setInt(int num);

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformPatricio ();
    virtual void enviaPosFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    virtual void resizeGL(int width, int height);
    virtual void initializeGL();

  private:
    int printOglError(const char file[], int line, const char func[]);

    float fovOptimo;
    int anglePatricio;
};
