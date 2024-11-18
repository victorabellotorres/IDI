#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent)
    : QLabel (parent) {
    //connect(this,SIGNAL(nomSignal()),this,SLOT(nomSlot()));
    // Inicialització d'atributs si cal
}
// implementació slots
void MyLabel::saveText(QString text) {
        str = text;
        setText(str);
        truncateText(trnc_num);
}

void MyLabel::truncateText(int num) {
        trnc_num = num;
        QString str_tmp = str;
        str_tmp.truncate(num);
        setText(str_tmp);
}