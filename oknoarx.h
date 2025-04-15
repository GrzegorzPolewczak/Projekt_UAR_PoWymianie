#ifndef OKNOARX_H
#define OKNOARX_H


#include <QDialog>
namespace Ui
{
class Oknoarx;
}

class Oknoarx : public QDialog
{
    Q_OBJECT

public:
    explicit Oknoarx(QWidget *parent = nullptr);
    ~Oknoarx();

    QString getWartoscA();
    QString getWartoscB();
    double getOpoznienie();
    double getZaklocenie();
    double getInterwal();

    void setWartoscA(QString wartosc);
    void setWartoscB(QString wartosc);
    void setZaklocenie(double zaklocenie);
    void setOpoznienie(double opoznienie);
    void setInterwal (double interwal);


private:
    Ui::Oknoarx *ui;
};
#endif // OKNOARX_H
