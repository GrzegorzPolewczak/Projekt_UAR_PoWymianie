#include "ModelARX.h"
#include <vector>
#include <numeric>

ModelARX::ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie, double zaklocenie_)
    : a(a_), b(b_), opoznienie_k(opoznienie), zaklocenie(zaklocenie_)
{
    bufor_sterowania = std::deque<double>(b.size() + opoznienie_k, 0.0);
    bufor_opoznienia = std::deque<double>(a.size(), 0.0);
}

ModelARX::ModelARX()
    : a({0.0}), b({0.0}), opoznienie_k(1), zaklocenie(0.0)
{
    bufor_sterowania = std::deque<double>(b.size() + opoznienie_k, 0.0);
    bufor_opoznienia = std::deque<double>(a.size(), 0.0);
}

double ModelARX::wykonajKrok(double wejscie) {

    const size_t requiredSize = opoznienie_k + b.size();

    bufor_sterowania.push_front(wejscie);

    if (bufor_sterowania.size() > requiredSize) {
        bufor_sterowania.pop_back();
    }

    double wyjscie = zaklocenie;

    for (size_t i = 0; i < b.size(); i++) {
       wyjscie += b[i] * bufor_sterowania[opoznienie_k + i];
    }

    for (size_t i = 0; i < a.size(); i++) {
        if (i < bufor_opoznienia.size()) {
            wyjscie -= a[i] * bufor_opoznienia[i];
        }
    }

    bufor_opoznienia.push_front(wyjscie);
    if (bufor_opoznienia.size() > a.size()) {
        bufor_opoznienia.resize(a.size());
    }

    return wyjscie;
}

void ModelARX::setZaklocenie(double zak)
{
    zaklocenie = zak;
}


