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

    virtual void carregaShaders ();
    virtual void resizeGL(int width, int heigth);

    void enviaPosFocus();
    void colocarBrick();
    void restart();

  public slots:
    void setSmallBrick(bool esSmall);
    void setMiddleBrick(bool esMiddle);
    void setBigBrick(bool esBig);

    void setRed(int);
    void setGreen(int);
    void setBlue(int);

  signals:
    void setSmallRButton();
    void setMiddleRButton();
    void setBigRButton();
    void setInt(int);

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    GLuint posFocusLoc;
    glm::vec3 posFocus;

    glm::vec3 red = glm::vec3(0.7, 0, 0);
    glm::vec3 black = glm::vec3(0, 0, 0);

    float dist;

    glm::vec3 currentBrickPos;

    int rotacio;

    glm::mat4 currentTG;

    float fovOptim;

    bool camara2;
};
