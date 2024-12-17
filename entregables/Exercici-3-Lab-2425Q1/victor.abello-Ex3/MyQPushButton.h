#include <QPushButton>

class MyQPushButton : public QPushButton
{
    Q_OBJECT

public slots:
    void rotateReleased();
    void changeColor(int position);
    void changeBrightness(int B);

signals:
    void sendPosition(int position);

public:
    MyQPushButton(QWidget *parent = 0);
private:

    enum colors{red = 0, magenta = 1, blue = 2, cyan = 3, green = 4, yellow = 5};
    static colors TopBottonColor; // the color of the top bottom will be the reference used to know the color of the other buttons

    static int Brightness;

    static const int rgb_colors[6][3];
};