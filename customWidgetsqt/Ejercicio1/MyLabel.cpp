#include "MyLabel.h"
#include <QObject>
#include <QPushButton>
#include <QDebug>

// constructor
MyLabel::MyLabel(QWidget *parent)
    : QLabel (parent) {
    //connect(this,SIGNAL(nomSignal()),this,SLOT(nomSlot()));
    // Inicialització d'atributs si cal
    motos = cotxes = camions = recaudat = 0;
    porta = Tancat;
}
// implementació slots
void MyLabel::passaVehicle() {
        QObject *obj = sender();
        QPushButton *button = qobject_cast<QPushButton *>(obj);
        if (button && porta == Obert) {
                if (button->text() == "Moto") {
                        qDebug() << "Moto";
                        motos++;
                        recaudat++; 
                        emit incrementarMoto(motos);
                        emit incrementarRecaudat(recaudat);
                } else if (button->text() == "Cotxe") {
                        // qDebug() << "Cotxe";
                        cotxes++;
                        recaudat++;
                        emit incrementarCotxe(cotxes);
                        emit incrementarRecaudat(recaudat);
                } else if (button->text() == "Camió") {
                        // qDebug() << "Camió";
                        camions++;
                        recaudat += 4;
                        emit incrementarCamio(camions);
                        emit incrementarRecaudat(recaudat);
                }
        }
}

void MyLabel::actualitzarPorta() {
        if (porta == Obert) {
                porta = Tancat;
                setStyleSheet("background: red;");
        }
        else {
                porta = Obert;
                setStyleSheet("background: green;");
        }
}