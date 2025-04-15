#pragma once

class RegulatorPID
{
	double wzmocnienie;
	double stala_calkowania;
	double stala_rozniczkowania;
	double suma_calkowita;
    double skladnik_wzmocnienia =0.0;
    double skladnik_calkowania =0.0;
    double skladnik_rozniczkowania =0.0;
    double poprzedniUchyb = 0.0;
    double wyjscie;
    double dGranicaAW;
    double gGranicaAW;
    bool antiwindup;
    bool czyAW;
    bool metoda_calkowania;

public:

    RegulatorPID(double k, double Ti = 0.0, double Td =0.0, double dG = -1000.0, double gG = 1000.0);
    RegulatorPID();
    double wykonajKrok(double uchyb);
    double getK();
    double getTi();
    double getTd();
    void setAW(bool aw);
	void reset();
    void setGranica(double dolna, double gorna);
    void setK(double k);
    void setTi(double Ti);
    void setTd(double Td);
    void ustawMetodeCalkowania(bool metoda);
};
