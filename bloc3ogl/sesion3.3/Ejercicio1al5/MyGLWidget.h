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
    virtual void paintGL();
    virtual void modelTransformPatricio();

    void carregaShadersFocus();

  private:
    int printOglError(const char file[], int line, const char func[]);

    glm::vec3 posFocusSCO, posFocusSCA, colorFocusSCA, colorFocusSCO;
    GLuint posFocusLocSCO, posFocusLocSCA, colorFocusLocSCA, colorFocusLocSCO;

    float displacementX, displacementZ;

    enum typeFocus {focusCamera, focusFix};
    typeFocus focus;


};
