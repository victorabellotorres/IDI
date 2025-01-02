#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    
    virtual void modelTransformCurrentBrick();

  public slots:
    
      void reiniciaSlot();
      void pecaPetitaSlot();
      void pecaMitjanaSlot();
      void pecaGranSlot();
      void canviaColor();
  signals:
        void pecaPetitaSignal();
      void pecaMitjanaSignal();
      void pecaGranSignal();

  private:
    int printOglError(const char file[], int line, const char func[]);
    float movX, movY, movZ, rotacio, l,r,u,d;
    void comprovaMesures();
    bool orthoCamera;
    int nbrick = 0;
    glm::mat4 currentTG;
    
};
