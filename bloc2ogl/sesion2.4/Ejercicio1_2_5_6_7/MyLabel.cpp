#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
    red = 0;
    green = 0;
    blue = 0;
    setStyleSheet("background: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ");");
}

MyLabel::~MyLabel() {}

void MyLabel::changeRed(int r) {
    red = r;
    setStyleSheet("background: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ");");
    emit sendFloorColor(red, green, blue);
    emit sendBGColor(red, green, blue);
}

void MyLabel::changeGreen(int g) {
    green = g;
    setStyleSheet("background: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ");");
    emit sendFloorColor(red, green, blue);
    emit sendBGColor(red, green, blue);
}

void MyLabel::changeBlue(int b) {
    blue = b;
    setStyleSheet("background: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ");");
    emit sendFloorColor(red, green, blue);
    emit sendBGColor(red, green, blue);
}


void MyLabel::setColor(int r, int g, int b) {
    red = r;
    green = g;
    blue = b;
    setStyleSheet("background: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ");");

    emit sendRed(red);
    emit sendGreen(green);
    emit sendBlue(blue);
}



