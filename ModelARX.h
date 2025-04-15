#pragma once
#include <vector>
#include <deque>
class ModelARX
{
private:
    std::vector<double> a;
    std::vector<double> b;
    std::deque<double> bufor_sterowania;
    std::deque<double> bufor_opoznienia;
    int opoznienie_k;
    double zaklocenie;

public:
    ModelARX(std::vector<double>& a_, std::vector<double>& b_, int opoznienie = 1, double zaklocenie_ = 0.0);
    ModelARX();
    std::vector<double> getA() {return a;}
    std::vector<double> getB() {return b;}
    int getOpoznienie() {return opoznienie_k;}
    double getZaklocenie() {return zaklocenie;}

    double wykonajKrok(double wejscie);
    void setZaklocenie(double zak);
    void setA(std::vector<double>& a_) {a = a_;}
    void setB(std::vector<double>& b_) {b = b_;}
    void setOpoznienie(int opoz) {opoznienie_k = opoz;}
    void setZaklocenie(int zak) { zaklocenie = zak; }

};

