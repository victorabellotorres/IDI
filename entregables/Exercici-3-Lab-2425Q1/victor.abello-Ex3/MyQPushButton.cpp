#include "MyQPushButton.h"

MyQPushButton::MyQPushButton(QWidget* parent): QPushButton(parent) {}

const int MyQPushButton::rgb_colors[6][3] = {{ 1 , 0 , 0 }, // red
                                             { 1 , 0 , 1 }, // magenta
                                             { 0 , 0 , 1 }, // blue
                                             { 0 , 1 , 1 }, // cyan
                                             { 0 , 1 , 0 }, // green
                                             { 1 , 1 , 0 }}; // yellow

int MyQPushButton::Brightness = 255;

MyQPushButton::colors MyQPushButton::TopBottonColor = red;

void MyQPushButton::rotateReleased()
{
    if (this->objectName() == "pushButton_Clockwise") {
        TopBottonColor = colors((((TopBottonColor - 1)%6)+6)%6); // this is the way to do a modulo operation in C++ that works with negative numbers
        emit sendPosition(0);
    }
    else if (this->objectName() == "pushButton_antiClockwise") {
        TopBottonColor = colors((TopBottonColor + 1)%6);
        emit sendPosition(0);
    }
}

void MyQPushButton::changeColor(int position)
{
    int c = (TopBottonColor + position)%6;
    this->setStyleSheet("background-color: rgb(" + QString::number(rgb_colors[c][0]*Brightness) + "," + QString::number(rgb_colors[c][1]*Brightness) + "," + QString::number(rgb_colors[c][2]*Brightness) + ");");
    emit sendPosition(position + 1);
}

void MyQPushButton::changeBrightness(int B) {
    Brightness = B;
    emit sendPosition(0);
}