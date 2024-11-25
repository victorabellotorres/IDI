#include <QLabel>


class MyLabel : public QLabel
{
    Q_OBJECT

    public:
        MyLabel(QWidget *parent=0);

        ~MyLabel();

    public slots:
    void changeRed(int r);
    void changeGreen(int g);
    void changeBlue(int b);
    void setColor(int r, int g, int b);

    signals:
    void sendFloorColor(int r, int g, int b);
    void sendBGColor(int r, int g, int b);
    
    void sendRed(int i);
    void sendGreen(int i);
    void sendBlue(int i);

    private:

    int red, green, blue;

};
