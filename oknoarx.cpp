#include "oknoarx.h"
#include "ui_oknoarx.h"
#include "ukladautomatycznejregulacji.h"


Oknoarx::Oknoarx(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Oknoarx)
{
    ui->setupUi(this);
    //POŁĄCZENIE PRZYCISKÓW Z OKNA DIALOGOWEGO
    connect(ui->btn_zatwierdz,&QPushButton::clicked,this,&Oknoarx::accept);
    connect(ui->btn_anuluj,&QPushButton::clicked,this,&Oknoarx::reject);
    //USTAWIENIE WARTOŚCI MIN I MAX ZAKŁÓCENIA I OPÓŹNIENIA
    ui->dspinbox_opoz->setMinimum(1.0);
    ui->dspinbox_opoz->setMaximum(1000.0);
    ui->dspinbox_zakl->setMinimum(0.0);
    ui->dspinbox_zakl->setMaximum(1000.0);
}

Oknoarx::~Oknoarx()
{
    delete ui;
}

//SETTERY DO WARTOŚCI ARX
void Oknoarx::setWartoscA(QString wartosc)
{
    ui->plaintextedit_a->setPlainText(wartosc);
}
void Oknoarx::setWartoscB(QString wartosc)
{
    ui->plaintextedit_b->setPlainText(wartosc);
}
void Oknoarx::setOpoznienie(double opoznienie)
{
    ui->dspinbox_opoz->setValue(opoznienie);
}
void Oknoarx::setZaklocenie(double zaklocenie)
{
    ui->dspinbox_zakl->setValue(zaklocenie);
}
void Oknoarx::setInterwal(double interwal)
{
    ui->dspinbox_inter->setValue(interwal);
}

//GETTERY DO WARTOŚCI ARX
QString Oknoarx::getWartoscA()
{
    return ui->plaintextedit_a->toPlainText();
}
QString Oknoarx::getWartoscB()
{
    return ui->plaintextedit_b->toPlainText();
}
double Oknoarx::getOpoznienie()
{
    return ui->dspinbox_opoz->value();
}
double Oknoarx::getZaklocenie()
{
    return ui->dspinbox_zakl->value();
}
double Oknoarx::getInterwal()
{
    return ui->dspinbox_inter->value();
}

