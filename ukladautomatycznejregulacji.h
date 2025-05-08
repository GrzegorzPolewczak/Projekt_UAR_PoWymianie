#ifndef UKLADAUTOMATYCZNEJREGULACJI_H
#define UKLADAUTOMATYCZNEJREGULACJI_H

#pragma once

#include <QMainWindow>
#include <QShortcut>
#include <QTimer>
#include "UkladSterowania.h"
#include "NetworkManager.h"
#include "ModelARX.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class UkladAutomatycznejRegulacji;
}
QT_END_NAMESPACE

class UkladAutomatycznejRegulacji : public QMainWindow
{
    Q_OBJECT

public:
    explicit UkladAutomatycznejRegulacji(QWidget *parent = nullptr);
    explicit UkladAutomatycznejRegulacji(QWidget *parent, ModelARX* model);
    ~UkladAutomatycznejRegulacji();

    double ostatniaWartoscObiektu = 0.0;
    bool odpowiedzOdebrana = false;

private slots:
    void on_zapisDoPliku_clicked();
    void on_wgrajzPliku_clicked();
    void on_symuluj_clicked();
    void on_wyczyscDane_clicked();
    void ZapisDoPliku();
    void WczytajzPliku();
    void startSymulacji();
    void ustawShortcuty();
    void ustawWykresy();
    void ustawARX(QString wektor_a, QString wektor_b, int opoznienie, double zaklocenie, double interwal);
    void ustawPID();
    void ustawGWZ();
    void on_zatrzymaj_clicked();
    void on_ukryjLegendy_clicked();
    void on_wgrajARX_clicked();
    void on_wgrajPID_clicked();
    void on_wgrajGWZ_clicked();
    void on_resetuj_clicked();
    void on_reset_calka_clicked();
    void on_btn_start_server_clicked();
    void on_btn_polacz_klient_clicked();
    void on_checkbox_trybSieciowy_stateChanged(int arg1);


    void rozlaczPolaczenie();

private:
    Ui::UkladAutomatycznejRegulacji *ui;
    UkladSterowania* us;
    QString text_a, text_b;
    double uchyb = 0.0;
    bool isZaklocenie = true;
    QTimer *timer;
    double time = 0.0;
    bool isLegenda = true;
    bool isWgrane[3] = {0, 0, 0};
    QString wektor_a, wektor_b;
    int opoznienie = 0;
    double zaklocenie = 0.0, interwal = 100.0;
    NetworkManager *manager;

};

#endif // UKLADAUTOMATYCZNEJREGULACJI_H
