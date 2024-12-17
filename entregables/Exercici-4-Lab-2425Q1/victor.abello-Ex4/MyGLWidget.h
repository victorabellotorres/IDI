// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT

  public slots:
    void modoDisco ();
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

    virtual void initializeGL ();
    virtual void paintGL ();
    virtual void modelTransformMorty ();
    virtual void modelTransformFantasma (float posX);
    virtual void modelTransformDiscoBall ();

    void carregaLocsFocus();
    void iniFocus();

   private:
    int printOglError(const char file[], int line, const char func[]);

    float girBailarins, girBola;

    //array con los colores rojo, verde, azul y amarillo en rgb 
    static const glm::vec3 colores[4];

    glm::vec3 posFocusBlanc, posFocusDisco[4];
    glm::vec3 colorFocusBlanc, colorFocusDisco[4];

    GLuint posFocusBlancLoc, posFocusDiscoLoc[4];
    GLuint colorFocusBlancLoc, colorFocusDiscoLoc[4];

    int indexLlum;

    QTimer timer;
};;
