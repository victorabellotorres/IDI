#include <QLabel>
class MyLabel: public QLabel
{
    Q_OBJECT
public:
    MyLabel (QWidget *parent);
public slots:
    void truncateText (int num);
    void saveText (QString);
private:
    QString str;
    int trnc_num;
};