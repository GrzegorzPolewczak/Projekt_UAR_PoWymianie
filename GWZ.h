#pragma once
#include <QString>

enum TypSygnalu { skok, sinusoida, prostokatny };
class GWZ {
private:
	double amplituda;
	int czas_aktywacji; //skok
	int aktualny_czas;
    TypSygnalu typ;
    double okres; //sinus
    double wypelnienie; //prostokatny
    double skladowa_stala;  // Dodajemy stałą S

public:
    GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_ = 0, double okres_ = 0.0, double wypelnienie_ = 0.0, double skladowa_stala_ = 0.0);
    GWZ();
    double pobierzWartoscZadana(double czas);
    void reset();
    void setTyp(TypSygnalu typ_) { typ = typ_;}
    void setAmplituda(double amplituda_) {amplituda = amplituda_;}
    void setOkres(double okres_) {okres = okres_;}
    void setWypelnienie(double wypelnienie_) {wypelnienie = wypelnienie_;}
    void setCzas(int czas_aktywacji_) {czas_aktywacji = czas_aktywacji_;}
    void setSkladowaStala(double S) { skladowa_stala = S; }

};
