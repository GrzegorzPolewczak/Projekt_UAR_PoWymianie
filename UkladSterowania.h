#pragma once
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "GWZ.h"
class UkladSterowania
{
private:

    double uchyb;
    double poprzedniUchyb;
public:
    ModelARX model;
    RegulatorPID regulator;
    GWZ gwz;
    UkladSterowania(ModelARX arx_model, RegulatorPID pid, GWZ gwz);
    UkladSterowania();
    double symuluj(double aktualne_wejscie);
    void setUchyb(double uchyb_);
    void setPoprzedniUchyb(double poprzedniUchyb_);
    double getUchyb();
    double getPoprzedniUchyb();
    void reset();
};

