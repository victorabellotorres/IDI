#include "MyQLineEdit.h"
#include <QObject>
#include <QDebug>

// constructor
MyQLineEdit::MyQLineEdit(QWidget *parent)
    : QLineEdit (parent) {
    //connect(this,SIGNAL(nomSignal()),this,SLOT(nomSlot()));
    // Inicialització d'atributs si cal
    intents = 5;
    paraulaSecreta = "DICCIONARI";
    previousSize = 0;
    setText("");
    
}
// implementació slots
void MyQLineEdit::lletraAfegida(QString str) {
    if (str.size() == 0 || str[str.size()-1] == paraulaSecreta[str.size()-1]) {
        if (str == paraulaSecreta) {
                setStyleSheet("background: green");
                //bloquear QLineEdit
                setText("");
                setReadOnly(true);
        }
        QString tmp_str = str;
        for (int i = str.size(); i < paraulaSecreta.size(); i++) {
            tmp_str += "*";
        }
        emit encertLletra(tmp_str);
    } else if (str.size() > 0 && previousSize < str.size()) {
        if (intents-1 == 0) {
                setStyleSheet("background: red");
                //bloquear QLineEdit
                setText("");
                setReadOnly(true);
        }
        emit errorLletra(--intents);
    }
    previousSize = str.size();
}

void MyQLineEdit::reset() {
    intents = 5;
    QString tmp_str = "";
    for (int i = 0; i < paraulaSecreta.size(); i++) {
        tmp_str += "*";
    }
    emit encertLletra(tmp_str);
    previousSize = 0;
    emit errorLletra(intents);
    setText("");
    setReadOnly(false);
    setStyleSheet("background: white");
}