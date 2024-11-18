#include <QLabel>
class MyLabel: public QLabel
{
    Q_OBJECT
public:
    MyLabel (QWidget *parent);
public slots:
    void passaVehicle ();
    void actualitzarPorta ();

signals:
    void incrementarMoto(int num);
    void incrementarCotxe(int num);
    void incrementarCamio(int num);
    void incrementarRecaudat(int num);

private: 

    int valorRecaudat, motos, cotxes, camions, recaudat;
    enum EstatPorta {
        Obert,
        Tancat
    };

    EstatPorta porta;

};