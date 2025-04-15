#include "ModelARX.h"
#include "RegulatorPID.h"
#include "UkladSterowania.h"

UkladSterowania::UkladSterowania(ModelARX arx_model, RegulatorPID pid, GWZ gwz)
    : model(arx_model), regulator(pid), gwz(gwz)
{
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}

UkladSterowania::UkladSterowania()
: model(), regulator(), gwz(){
    uchyb = 0.0;
    poprzedniUchyb = 0.0;
}

//git
double UkladSterowania::symuluj(double aktualne_wejscie) {
    setUchyb(aktualne_wejscie - getPoprzedniUchyb());

    double wyjscie_model = model.wykonajKrok(regulator.wykonajKrok(getUchyb()));
    setPoprzedniUchyb(wyjscie_model);
    return wyjscie_model;
}

void UkladSterowania::setUchyb(double uchyb_)
{
    uchyb = uchyb_;
}

void UkladSterowania::setPoprzedniUchyb(double poprzedniUchyb_)
{
    poprzedniUchyb = poprzedniUchyb_;
}

double UkladSterowania::getUchyb()
{
    return uchyb;
}

double UkladSterowania::getPoprzedniUchyb()
{
    return poprzedniUchyb;
}

void UkladSterowania::reset()
{
    poprzedniUchyb = 0.0;
    uchyb = 0.0;
}
