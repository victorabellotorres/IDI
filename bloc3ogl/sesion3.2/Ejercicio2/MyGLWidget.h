// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void iniMaterialTerra();  
    virtual void initializeGL();

    void carregaShadersFocus();

  private:
    int printOglError(const char file[], int line, const char func[]);

    glm::vec3 posFocus, colorFocus;

    GLuint posFocusLoc, colorFocusLoc;


};
