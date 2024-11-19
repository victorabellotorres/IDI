#include <QLineEdit>
class MyQLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    MyQLineEdit (QWidget *parent);
public slots:
    void lletraAfegida(QString str);
    void reset();

signals:
    void errorLletra(int num);
    void encertLletra(QString str);
    
    

private: 

    int intents, previousSize;
    QString paraulaSecreta;
};