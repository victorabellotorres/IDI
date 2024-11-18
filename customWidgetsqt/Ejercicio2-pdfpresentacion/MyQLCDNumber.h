#include <QLCDNumber>
class MyQLCDNumber: public QLCDNumber
{
    Q_OBJECT
public:
    MyQLCDNumber (QWidget *parent);
public slots:
    virtual void display(int num);

    void actualitzarColors (int num);
    void setZero ();
private:
};