#include "MyQLCDNumber.h"
// constructor
MyQLCDNumber::MyQLCDNumber(QWidget *parent)
    : QLCDNumber (parent) {
    //connect(this,SIGNAL(nomSignal()),this,SLOT(nomSlot()));
    // Inicialització d'atributs si cal
    setStyleSheet("color: rgb(0, 255, 0);");
}

// implementació slots
void MyQLCDNumber::display(int num) {
        QLCDNumber::display(num);
        actualitzarColors(num);
}


void MyQLCDNumber::actualitzarColors(int num) {
        if (num == 0) setStyleSheet("color: rgb(0, 255, 0);");// verd
        else if (num%2 == 0) setStyleSheet("color: rgb(0, 0, 255);");
        else setStyleSheet("color: rgb(255, 0, 0);");
}

void MyQLCDNumber::setZero() {
        display(0);
}