#include "GWZ.h"
#include <QtMath>

GWZ::GWZ(TypSygnalu typ_, double amplituda_, int czas_aktywacji_, double okres_, double wypelnienie_, double skladowa_stala_)
    : amplituda(amplituda_), czas_aktywacji(czas_aktywacji_), typ(typ_), okres(okres_), wypelnienie(wypelnienie_), skladowa_stala(skladowa_stala_)
{}


GWZ::GWZ() {}

double GWZ::pobierzWartoscZadana(double t)
{
    if (t < czas_aktywacji) {
        return skladowa_stala;  // brak sygnału przed czasem aktywacji
    }

    double czas_od_startu = t - czas_aktywacji;

    switch (typ) {

    case TypSygnalu::skok:
        return skladowa_stala + amplituda;

    case TypSygnalu::sinusoida:
        return skladowa_stala + (amplituda * sin((2 * M_PI / okres) * czas_od_startu));

    case TypSygnalu::prostokatny: {
        double t_modulo = fmod(czas_od_startu, okres);

        if (t_modulo < okres * wypelnienie) {
            return skladowa_stala + amplituda;
        } else {
            return skladowa_stala;
        }
    }

    default:
        return skladowa_stala;
    }
}



void GWZ::reset() {
	aktualny_czas = 0;
}

