#include "ukladautomatycznejregulacji.h"
#include "oknoarx.h"
#include "ui_ukladautomatycznejregulacji.h"

const QString nazwa = "konfiguracja.txt";

UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);
    ui->lampkaStatus->setFixedSize(20, 20);
    ui->lampkaStatus->setVisible(true);
    ui->lampkaStatus->setStyleSheet(
        "background-color: #444;"
        "border: 2px solid black;"
        "border-radius: 10px;"
        "min-width: 20px; min-height: 20px;"
        "max-width: 20px; max-height: 20px;"
        "margin: 2px;"
        "box-shadow: 0 0 5px rgba(0,0,0,0.5);"
        );
    on_checkbox_trybSieciowy_stateChanged(0);
    us = new UkladSterowania();
    setFixedSize(1400, 780);
    ustawShortcuty();
    ustawWykresy();
    ui->gorna->setMaximum(1000);
    ui->dolna->setMinimum(-1000);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UkladAutomatycznejRegulacji::startSymulacji);
    ui->zatrzymaj->setEnabled(false);
    ui->ukryjLegendy->setEnabled(false);
    ui->btn_polacz_klient->setEnabled(false);
    ui->btn_start_server->setEnabled(false);
    ui->btn_rozlacz->setEnabled(false);

    manager = new NetworkManager(this);

    connect(manager, &NetworkManager::connectedToPeer, this, [=]() {
        ui->lbAdresPolaczenia->setText(manager->peerAddressString());
        QMessageBox::information(this, "Połączono", "Połączono z serwerem");
        ui->wgrajARX->setEnabled(false);
        ui->wgrajGWZ->setEnabled(true);
        ui->wgrajPID->setEnabled(true);
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(true);
        ui->resetuj->setEnabled(true);
        ui->zapisDoPliku->setEnabled(false);
        ui->wgrajzPliku->setEnabled(false);
        ui->reset_calka->setEnabled(true);

        ui->btn_polacz_klient->setEnabled(false);
        ui->btn_start_server->setEnabled(false);
        ui->btn_rozlacz->setEnabled(true);
    });

    connect(manager, &NetworkManager::clientConnected, this, [=]() { 
        QMessageBox::information(this, "Klient połączony", "Klient połączył się z serwerem");
        ui->wgrajARX->setEnabled(true);
        ui->wgrajGWZ->setEnabled(false);
        ui->wgrajPID->setEnabled(false);
        ui->symuluj->setEnabled(false);
        ui->zatrzymaj->setEnabled(false);
        ui->resetuj->setEnabled(false);
        ui->zapisDoPliku->setEnabled(false);
        ui->wgrajzPliku->setEnabled(false);
        ui->reset_calka->setEnabled(false);

        ui->btn_polacz_klient->setEnabled(false);
        ui->btn_start_server->setEnabled(false);
        ui->btn_rozlacz->setEnabled(true);

        ui->lbAdresPolaczenia->setText(manager->peerAddressString());
    });

    connect(manager, &NetworkManager::connectionFailed, this, [=](QString reason) {
        ui->lbAdresPolaczenia->setText(reason);
        QMessageBox::critical(this, "Błąd połączenia", reason);
    });

    connect(manager, &NetworkManager::otrzymanoWyjscie, this, [=](double wyjscie_arx){

        ostatniaWartoscObiektu = wyjscie_arx;
        odpowiedzOdebrana = true;
    });

    connect(manager, &NetworkManager::connectionLost, this, [=](){
        QMessageBox::critical(this, "Zerwano połączenie", "Połączenie sieciowe zostało przerwane.\nPrzechodzę do trybu lokalnego.");
        ui->checkbox_trybSieciowy->blockSignals(true);
        ui->checkbox_trybSieciowy->setChecked(false);
        ui->checkbox_trybSieciowy->blockSignals(false);
        ui->btn_rozlacz->setEnabled(false);
        on_checkbox_trybSieciowy_stateChanged(0);

        ui->lbAdresPolaczenia->clear();
    });

    connect(ui->btn_rozlacz, &QPushButton::clicked, this, &UkladAutomatycznejRegulacji::rozlaczPolaczenie);
}


UkladAutomatycznejRegulacji::UkladAutomatycznejRegulacji(QWidget *parent, ModelARX* model)
    : QMainWindow(parent)
    , ui(new Ui::UkladAutomatycznejRegulacji)
{
    ui->setupUi(this);

    manager = new NetworkManager(this);
    if (model != nullptr)
        manager->setModel(model);

    on_checkbox_trybSieciowy_stateChanged(0);
    us = new UkladSterowania();
    setFixedSize(1400, 780);
    ustawShortcuty();
    ustawWykresy();
    ui->gorna->setMaximum(1000);
    ui->dolna->setMinimum(-1000);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UkladAutomatycznejRegulacji::startSymulacji);
    ui->zatrzymaj->setEnabled(false);
    ui->ukryjLegendy->setEnabled(false);
    ui->btn_polacz_klient->setEnabled(false);
    ui->btn_start_server->setEnabled(false);
    ui->btn_rozlacz->setEnabled(false);

    connect(manager, &NetworkManager::connectedToPeer, this, [=]() {
        QMessageBox::information(this, "Połączono", "Połączono z serwerem");
        ui->wgrajARX->setEnabled(false);
        ui->wgrajGWZ->setEnabled(true);
        ui->wgrajPID->setEnabled(true);
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(true);
        ui->resetuj->setEnabled(true);
        ui->zapisDoPliku->setEnabled(false);
        ui->wgrajzPliku->setEnabled(false);
        ui->reset_calka->setEnabled(true);
        ui->btn_polacz_klient->setEnabled(false);
        ui->btn_start_server->setEnabled(false);
        ui->btn_rozlacz->setEnabled(true);

        ui->lbAdresPolaczenia->setText(manager->peerAddressString());
    });

    connect(manager, &NetworkManager::clientConnected, this, [=]() {
        QMessageBox::information(this, "Klient połączony", "Klient połączył się z serwerem");
        ui->wgrajARX->setEnabled(true);
        ui->wgrajGWZ->setEnabled(false);
        ui->wgrajPID->setEnabled(false);
        ui->symuluj->setEnabled(false);
        ui->zatrzymaj->setEnabled(false);
        ui->resetuj->setEnabled(false);
        ui->zapisDoPliku->setEnabled(false);
        ui->wgrajzPliku->setEnabled(false);
        ui->reset_calka->setEnabled(false);
        ui->btn_polacz_klient->setEnabled(false);
        ui->btn_start_server->setEnabled(false);
        ui->btn_rozlacz->setEnabled(true);

        ui->lbAdresPolaczenia->setText(manager->peerAddressString());
    });

    connect(manager, &NetworkManager::connectionFailed, this, [=](QString reason) {
        QMessageBox::critical(this, "Błąd połączenia", reason);
    });

    connect(manager, &NetworkManager::otrzymanoWyjscie, this, [=](double wyjscie_arx){
        ostatniaWartoscObiektu = wyjscie_arx;
        odpowiedzOdebrana = true;
    });

    connect(manager, &NetworkManager::connectionLost, this, [=](){
        QMessageBox::critical(this, "Zerwano połączenie", "Połączenie sieciowe zostało przerwane.\nPrzechodzę do trybu lokalnego.");
        ui->checkbox_trybSieciowy->blockSignals(true);
        ui->checkbox_trybSieciowy->setChecked(false);
        ui->checkbox_trybSieciowy->blockSignals(false);
        ui->btn_rozlacz->setEnabled(false);
        on_checkbox_trybSieciowy_stateChanged(0);

        ui->lbAdresPolaczenia->clear();
    });

    connect(ui->btn_rozlacz, &QPushButton::clicked, this, &UkladAutomatycznejRegulacji::rozlaczPolaczenie);


    connect(manager, &NetworkManager::otrzymanoSterowanie,
            this, &UkladAutomatycznejRegulacji::onSterowanieReceived);
    connect(manager, &NetworkManager::otrzymanoWartoscZadana,
            this, &UkladAutomatycznejRegulacji::onWartoscZadanaReceived);

    connect(manager, &NetworkManager::otrzymanoWyjscie,
            this, &UkladAutomatycznejRegulacji::onRegulatedValueReceived);

    connect(manager, &NetworkManager::otrzymanoInterwal, this, &UkladAutomatycznejRegulacji::onOtrzymanoInterwal);
}

void UkladAutomatycznejRegulacji::rozlaczPolaczenie()
{
    auto confirm = QMessageBox::question(this,("Potwierdzenie rozłączenia"),("Czy na pewno chcesz zakończyć połączenie sieciowe?"),QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes)
    {
       return;
    }

    if (manager)
    {
        manager->disconnectFromHost();
        manager->stopServer();
    }

    ui->checkbox_trybSieciowy->setChecked(false);
    QMessageBox::information(this, "Rozłączono", "Połączenie zostało zakończone.");
    ui->btn_rozlacz->setEnabled(false);
}


UkladAutomatycznejRegulacji::~UkladAutomatycznejRegulacji()
{
    delete ui;
}

void UkladAutomatycznejRegulacji::rescaleVisibleY(QCustomPlot* plot)
{
    double minX = plot->xAxis->range().lower;
    double maxX = plot->xAxis->range().upper;

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (int i = 0; i < plot->graphCount(); ++i)
    {
        auto graph = plot->graph(i);
        auto dataMap = graph->data();

        for (auto it = dataMap->constBegin(); it != dataMap->constEnd(); ++it)
        {
            double x = it->key;
            double y = it->value;

            if (x >= minX && x <= maxX)
            {
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
    }

    if (minY < maxY)
    {
        double margin = 0.1 * std::max(std::abs(minY), std::abs(maxY));
        plot->yAxis->setRange(minY - margin, maxY + margin);
    }
}

void UkladAutomatycznejRegulacji::oczyscDanePozaZakresem(QCustomPlot* wykres)
{
    double minX = wykres->xAxis->range().lower - 1.0;
    double maxX = wykres->xAxis->range().upper + 1.0;

    for (int i = 0; i < wykres->graphCount(); ++i)
    {
        auto graph = wykres->graph(i);
        graph->data()->removeBefore(minX);
        graph->data()->removeAfter(maxX);
    }
}

void UkladAutomatycznejRegulacji::startSymulacji()
{
    time += interwal/1000.0;

    double wartZadana = us->gwz.pobierzWartoscZadana(time);
    double wyjscie_pid = us->regulator.wykonajKrok(us->getUchyb());


    double wyjscie_modelu;

    if (ui->checkbox_trybSieciowy->isChecked())
    {
        odpowiedzOdebrana = false;
        manager->sendSterowanie(wartZadana,wyjscie_pid);

        QElapsedTimer timeout;
        timeout.start();

        while (!odpowiedzOdebrana && timeout.elapsed() < 300) {
           QCoreApplication::processEvents();
        }

        if (odpowiedzOdebrana) {
            ui->lampkaStatus->setStyleSheet(
                "background-color: green;"
                "border: 2px solid black;"
                "border-radius: 10px;"
                "min-width: 20px; min-height: 20px;"
                "max-width: 20px; max-height: 20px;"
                );
            ui->lampkaStatus->setToolTip("Status komunikacji: odpowiedź na czas");
        } else {
            ui->lampkaStatus->setStyleSheet(
                "background-color: red;"
                "border: 2px solid black;"
                "border-radius: 10px;"
                "min-width: 20px; min-height: 20px;"
                "max-width: 20px; max-height: 20px;"
                );
            ui->lampkaStatus->setToolTip("Status komunikacji: brak odpowiedzi");
        }

        wyjscie_modelu = ostatniaWartoscObiektu;
        uchyb = wartZadana - wyjscie_modelu;
        us->setUchyb(uchyb);
    }
    else
    {

        double wartosc_zaklocenia = 0.0;

        if (zaklocenie != 0.0)
        {
            std::random_device srng;
            std::mt19937 rng;
            rng.seed(srng());
            std::normal_distribution<double> rozkladnormalny(0.0, zaklocenie);
            wartosc_zaklocenia = rozkladnormalny(rng);
        }

        wyjscie_modelu = us->model.wykonajKrok(wartZadana) + wartosc_zaklocenia;

        uchyb = wartZadana - wyjscie_modelu;
        us->setUchyb(uchyb);

        ui->lampkaStatus->setStyleSheet(
            "background-color: green;"
            "border: 2px solid black;"
            "border-radius: 10px;"
            "min-width: 20px; min-height: 20px;"
            "max-width: 20px; max-height: 20px;"
            );
        ui->lampkaStatus->setToolTip("Tryb lokalny – brak połączenia sieciowego");
    }



    ui->customPlot->graph(0)->addData(time, wyjscie_modelu);
    ui->customPlot->graph(1)->addData(time, wartZadana);
    ui->customPlot_uchyb->graph(0)->addData(time, uchyb);
    ui->customPlot_pid->graph(0)->addData(time, wyjscie_pid);
    ui->customPlot_pid->graph(1)->addData(time, us->regulator.getK());
    ui->customPlot_pid->graph(2)->addData(time, us->regulator.getTi());
    ui->customPlot_pid->graph(3)->addData(time, us->regulator.getTd());


    if (time > ui->customPlot->xAxis->range().upper)
    {
        ui->customPlot->xAxis->setRange(time, 10, Qt::AlignRight);
        ui->customPlot_uchyb->xAxis->setRange(time, 10, Qt::AlignRight);
        ui->customPlot_pid->xAxis->setRange(time, 10, Qt::AlignRight);
    }


    oczyscDanePozaZakresem(ui->customPlot);
    oczyscDanePozaZakresem(ui->customPlot_pid);
    oczyscDanePozaZakresem(ui->customPlot_uchyb);

    rescaleVisibleY(ui->customPlot);
    rescaleVisibleY(ui->customPlot_pid);
    rescaleVisibleY(ui->customPlot_uchyb);



    ui->customPlot->replot();
    ui->customPlot_pid->replot();

    ui->customPlot_uchyb->replot();

}


void UkladAutomatycznejRegulacji::closeEvent(QCloseEvent *event)
{
    if (manager && (manager->isClient() || manager->isServer())) {
        rozlaczPolaczenie();
    }

    event->accept();
}

void UkladAutomatycznejRegulacji::on_symuluj_clicked()
{

    bool daneOK = true;

    if (!ui->checkbox_trybSieciowy->isChecked()) {
        for (int i = 0; i < 3; i++) {
            if (isWgrane[i] == 0) {
                daneOK = false;
            }
        }
    }

    else {
        if (isWgrane[1] == 0) {
            daneOK = false;
        }
        if (isWgrane[2] == 0) {
            daneOK = false;
        }
    }

    if (daneOK)
    {
        if (!timer->isActive()) {
            int interwalCzasowy = interwal;
            if (!ui->checkbox_trybSieciowy->isChecked() || (manager && manager->isClient() && !manager->isServer())) {
                timer->start(interwalCzasowy);
            }
            //timer->start(interwalCzasowy);
            ui->symuluj->setEnabled(false);
            ui->zatrzymaj->setEnabled(true);
            ui->resetuj->setEnabled(true);
        }
        ui->ukryjLegendy->setEnabled(true);
    }
    else
    {
        if (!ui->checkbox_trybSieciowy->isChecked() && isWgrane[0] == false) {
            QMessageBox::warning(this, "Błąd startu symulacji", "Brakuje danych MODELU ARX.");
        }
        if (isWgrane[1] == false) {
            QMessageBox::warning(this, "Błąd startu symulacji", "Brakuje danych REGULATORA PID.");
        }
        if (isWgrane[2] == false) {
            QMessageBox::warning(this, "Błąd startu symulacji", "Brakuje danych GENERATORA WARTOŚCI ZADANEJ.");
        }
    }

}

void UkladAutomatycznejRegulacji::on_zatrzymaj_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(false);
        ui->resetuj->setEnabled(true);
    }
}



void UkladAutomatycznejRegulacji::on_resetuj_clicked()
{
    if (timer->isActive())
    {
        timer->stop();
    }

    time = 0.0;
    uchyb = 0.0;

    ui->customPlot->graph(0)->data()->clear();
    ui->customPlot->graph(1)->data()->clear();
    ui->customPlot_uchyb->graph(0)->data()->clear();
    ui->customPlot_pid->graph(0)->data()->clear();
    ui->customPlot_pid->graph(1)->data()->clear();
    ui->customPlot_pid->graph(2)->data()->clear();
    ui->customPlot_pid->graph(3)->data()->clear();

    ui->customPlot->replot();
    ui->customPlot_uchyb->replot();
    ui->customPlot_pid->replot();


    ui->symuluj->setEnabled(true);
    ui->zatrzymaj->setEnabled(false);
    ui->resetuj->setEnabled(false);
    ui->ukryjLegendy->setEnabled(false);

    ui->te_k->clear();
    ui->te_ti->clear();
    ui->te_td->clear();
    ui->amplituda->clear();
    ui->czas_aktywacji->clear();
    ui->wypelnienie->clear();
    ui->okres->clear();
    ui->gorna->clear();
    ui->dolna->clear();
    us->model.setZaklocenie(0.0);
}

void UkladAutomatycznejRegulacji::on_wyczyscDane_clicked()
{ 
    ui->te_k->clear();
    ui->te_ti->clear();
    ui->te_td->clear();
    ui->amplituda->clear();
    ui->czas_aktywacji->clear();
    ui->wypelnienie->clear();
    ui->okres->clear();
    QString skok = "skok";
    int index = ui->comboGWZ->findText(skok);
    ui->comboGWZ->setCurrentIndex(index);
    ui->gorna->clear();
    ui->dolna->clear();
}



void UkladAutomatycznejRegulacji::ustawPID()
{
    double wzmocnienie = ui->te_k->value();
    double stala_calkowania = ui->te_ti->value();
    double stala_rozniczkowania = ui->te_td->value();
    double gorna = ui->gorna->value();
    double dolna = ui->dolna->value();
    bool czyAW = ui->antiwindup->isChecked();
    bool metodaCalkowania = ui->checkbox_calkowanie->isChecked();
    us->regulator.setK(wzmocnienie);
    us->regulator.setTi(stala_calkowania);
    us->regulator.setTd(stala_rozniczkowania);
    us->regulator.setGranica(dolna, gorna);
    us->regulator.setAW(czyAW);
    us->regulator.ustawMetodeCalkowania(metodaCalkowania);

}

void UkladAutomatycznejRegulacji::ustawGWZ()
{
    TypSygnalu typ;
    QString typSygnalu = ui->comboGWZ->currentText();
    if(typSygnalu == "skok")
    {
        typ = TypSygnalu::skok;
    }
    if(typSygnalu == "sinusoida")
    {
        typ = TypSygnalu::sinusoida;
    }
    if(typSygnalu == "prostokatny")
    {
        typ = TypSygnalu::prostokatny;
    }
    double amplituda = ui->amplituda->value();
    int czas = ui->czas_aktywacji->value();
    double okres = ui->okres->value();
    double wypelnienie = ui->wypelnienie->value();
    double skladowa_stala = ui->skladowa_stala->value();

    us->gwz.setTyp(typ);
    us->gwz.setAmplituda(amplituda);
    us->gwz.setOkres(okres);
    us->gwz.setWypelnienie(wypelnienie);
    us->gwz.setCzas(czas);
    us->gwz.setSkladowaStala(skladowa_stala);

}

void UkladAutomatycznejRegulacji::on_zapisDoPliku_clicked()
{
    ZapisDoPliku();
}

void UkladAutomatycznejRegulacji::ZapisDoPliku()
{
    bool ok;
    QFile plik(nazwa);
    if (!plik.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Błąd otwarcia pliku", "Program nie mógł otworzyć pliku w celu zapisu konfiguracji do pliku", QMessageBox::Ok);
    }
    QTextStream out(&plik);
    if(!(wektor_a.isEmpty()))
    {
        out << "a: " << wektor_a.toDouble(&ok) << "\n";
    }
    else
    {
        out << "a: " << 0 << "\n";
    }
    if(!(wektor_b.isEmpty()))
    {
        out << "b: " << wektor_b.toDouble(&ok) << "\n";
    }
    else
    {
        out << "b: " << 0 << "\n";
    }
    out << "opoznienie: " << opoznienie<< "\n";
    out << "zaklocenie: " << zaklocenie << "\n";
    out << "k: " << ui->te_k->value() << "\n";
    out << "Ti: " << ui->te_ti->value() << "\n";
    out << "Td: " << ui->te_td->value() << "\n";
    out << "amplituda: " << ui->amplituda->value() << "\n";
    out << "wypelnienie: " << ui->wypelnienie->value() << "\n";
    out << "czas_aktywacji: " << ui->czas_aktywacji->value() << "\n";
    out << "okres: " << ui->okres->value() << "\n";
    out << "skladowa_stala: " << ui->skladowa_stala->value() << "\n";
    out << "typ: " << ui->comboGWZ->currentText()<< "\n";
    out << "dolna: " << ui->dolna->value()<< "\n";
    out << "gorna: " << ui->gorna->value() << "\n";
    QMessageBox::information(this, "Zapis do pliku", "Konfiguracja została zapisana do pliku", QMessageBox::Ok);
    plik.close();
}

void UkladAutomatycznejRegulacji::on_wgrajzPliku_clicked()
{
    WczytajzPliku();
}

void UkladAutomatycznejRegulacji::WczytajzPliku()
{
    QFile plik(nazwa);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Błąd otwarcia pliku", "Program nie mógł otworzyć pliku w celu wczytania konfiguracji", QMessageBox::Ok);
        return;
    }
    QTextStream in(&plik);
    while (!in.atEnd()) {
        QString linia = in.readLine();
        if (linia.startsWith("a:")) {
            wektor_a = linia.mid(2).trimmed();
        } else if (linia.startsWith("b:")) {
            wektor_b = linia.mid(2).trimmed();
        } else if (linia.startsWith("opoznienie:")) {
            opoznienie = (linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("zaklocenie:")) {
            zaklocenie = (linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("k:")) {
            ui->te_k->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("Ti:")) {
            ui->te_ti->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("Td:")) {
            ui->te_td->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("amplituda:")) {
            ui->amplituda->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("wypelnienie:")) {
            ui->wypelnienie->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("czas_aktywacji:")) {
            ui->czas_aktywacji->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("okres:")) {
            ui->okres->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if (linia.startsWith("skladowa_stala:")) {
            ui->skladowa_stala->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if(linia.startsWith("typ: ")){
            QString typ = linia.section(':', 1).trimmed();
            int index = ui->comboGWZ->findText(typ);
            if(index != -1)
                ui->comboGWZ->setCurrentIndex(index);
        } else if(linia.startsWith("dolna: ")){
            ui->dolna->setValue(linia.section(':', 1).trimmed().toDouble());
        } else if(linia.startsWith("gorna: ")){
            ui->gorna->setValue(linia.section(':', 1).trimmed().toDouble());
        }
    }
    plik.close();
}

void UkladAutomatycznejRegulacji::ustawWykresy()
{

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::red, 1.5));
    // Wartość Zadana
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::blue, 1.5));

    // Uchyb
    ui->customPlot_uchyb->addGraph();
    ui->customPlot_uchyb->graph(0)->setPen(QPen(Qt::green, 1.5));

    // Sterowanie
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(0)->setPen(QPen(Qt::blue));
    // Wzmocnienie
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(1)->setPen(QPen(Qt::magenta));
    // Ti - stała całkowania
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(2)->setPen(QPen(Qt::red));
    // Td - stała różniczkowania
    ui->customPlot_pid->addGraph();
    ui->customPlot_pid->graph(3)->setPen(QPen(Qt::green));


    ui->customPlot->legend->setVisible(true);
    ui->customPlot->graph(0)->setName("Wartość Regulowana");
    ui->customPlot->graph(1)->setName("Wartość Zadana");

    ui->customPlot_pid->legend->setVisible(true);
    ui->customPlot_pid->graph(0)->setName("Sterowanie");
    ui->customPlot_pid->graph(1)->setName("Wzmocnienie");
    ui->customPlot_pid->graph(2)->setName("Stała Całkowania");
    ui->customPlot_pid->graph(3)->setName("Stała Różniczkowania");


    ui->customPlot->xAxis->setLabel("Czas [s]");
    ui->customPlot->yAxis->setLabel("Wyjście");
    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(-5, 5);

    ui->customPlot_uchyb->xAxis->setLabel("Czas [s]");
    ui->customPlot_uchyb->yAxis->setLabel("Uchyb");
    ui->customPlot_uchyb->xAxis->setRange(0, 10);
    ui->customPlot_uchyb->yAxis->setRange(-5, 5);


    ui->customPlot_pid->xAxis->setLabel("Czas [s]");
    ui->customPlot_pid->xAxis->setRange(0, 10);
    ui->customPlot_pid->yAxis->setRange(-5, 5);
}

void UkladAutomatycznejRegulacji::on_ukryjLegendy_clicked()
{
    if(isLegenda == true)
    {
        ui->customPlot->legend->setVisible(false);
        ui->customPlot_pid->legend->setVisible(false);
        ui->customPlot_uchyb->legend->setVisible(false);
        isLegenda = false;
        ui->ukryjLegendy->setText("Pokaż legendy");
    }
    else
    {
        ui->customPlot->legend->setVisible(true);
        ui->customPlot_pid->legend->setVisible(true);
        ui->customPlot_uchyb->legend->setVisible(true);
        isLegenda = true;
        ui->ukryjLegendy->setText("Ukryj legendy");
    }
}

void UkladAutomatycznejRegulacji::on_wgrajARX_clicked()
{
    Oknoarx okienko(this);

    if (okienko.exec() == QDialog::Accepted)
    {

        wektor_a = okienko.getWartoscA();
        wektor_b = okienko.getWartoscB();
        opoznienie = okienko.getOpoznienie();
        zaklocenie = okienko.getZaklocenie();
        interwal = okienko.getInterwal();

        isWgrane[0] = 1;
        ustawARX(wektor_a, wektor_b, opoznienie, zaklocenie, interwal);

        if (ui->checkbox_trybSieciowy->isChecked() && manager && manager->isServer()) {
            manager->setInterwal(interwal);
            manager->sendInterwal(interwal);
        }

    }

}

void UkladAutomatycznejRegulacji::on_wgrajPID_clicked()
{
    ustawPID();
    isWgrane[1] = 1;
}

void UkladAutomatycznejRegulacji::on_wgrajGWZ_clicked()
{
    ustawGWZ();
    isWgrane[2] = 1;
}

void UkladAutomatycznejRegulacji::on_reset_calka_clicked()
{
    us->regulator.reset();
}


void UkladAutomatycznejRegulacji::ustawARX(QString wektor_a, QString wektor_b, int opoznienie, double zaklocenie, double interwal)
{
    bool ok;
    std::vector<double> a;
    std::vector<double> b;

    QStringList aList = wektor_a.split(" ");
    QStringList bList = wektor_b.split(" ");

    for (const QString &a_i : aList)
    {
        double value = a_i.toDouble(&ok);
        if (ok)
            a.push_back(value);
        else
            QMessageBox::warning(this, "Błąd wartości", "Podaj poprawną wartość wektora A!", QMessageBox::Ok);
    }

    for (const QString &b_i : bList)
    {
        double value = b_i.toDouble(&ok);
        if (ok)
            b.push_back(value);
        else
            QMessageBox::warning(this, "Błąd wartości", "Podaj poprawną wartość wektora B!", QMessageBox::Ok);
    }

    us->model.setA(a);
    us->model.setB(b);
    us->model.setOpoznienie(opoznienie);
    us->model.setZaklocenie(zaklocenie);

    timer->setInterval(static_cast<int>(interwal));

    qDebug() << "Wektor A:" << a;
    qDebug() << "Wektor B:" << b;

    if (ui->checkbox_trybSieciowy->isChecked()) {
        manager->setModel(&(us->model));
        qDebug() << "[ustawARX] Przypisano model do NetworkManager";
    }
}


void UkladAutomatycznejRegulacji::ustawShortcuty()
{
    QShortcut* zapis_skrot = new QShortcut(QKeySequence("Ctrl+S"), this);
    QShortcut* wczytaj_skrot = new QShortcut(QKeySequence("Ctrl+L"), this);
    QShortcut* start_skrot = new QShortcut(QKeySequence("Ctrl+F2"),this);
    QShortcut* stop_skrot = new QShortcut(QKeySequence("Ctrl+F3"), this);
    QShortcut* wyczysc_skrot = new QShortcut(QKeySequence("Ctrl+F4"), this);
    QShortcut* reset_skrot = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(zapis_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::ZapisDoPliku);
    connect(wczytaj_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::WczytajzPliku);
    connect(start_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_symuluj_clicked);
    connect(stop_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_zatrzymaj_clicked);
    connect(wyczysc_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_wyczyscDane_clicked);
    connect(reset_skrot, &QShortcut::activated, this, &UkladAutomatycznejRegulacji::on_resetuj_clicked);
}

void UkladAutomatycznejRegulacji::on_btn_start_server_clicked()
{
    bool ok;
    int port = QInputDialog::getInt(this, "Port", "Podaj port serwera:", 12345, 0, 65535, 1, &ok);
    if (ok) {
        if (manager->startServer(port)) {
            QMessageBox::information(this, "Serwer", "Serwer nasłuchuje na porcie " + QString::number(port));
        }
    }

}


void UkladAutomatycznejRegulacji::on_btn_polacz_klient_clicked()
{
    bool ok;
    QString host = QInputDialog::getText(this, "Adres IP", "Podaj adres IP:", QLineEdit::Normal, "127.0.0.1", &ok);
    if (!ok || host.isEmpty()) return;

    int port = QInputDialog::getInt(this, "Port", "Podaj port:", 12345, 0, 65535, 1, &ok);
    if (ok) {
        manager->connectToServer(host, port);
    }
}


void UkladAutomatycznejRegulacji::on_checkbox_trybSieciowy_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Aktywacja trybu sieciowego",
                                                                    "Czy na pewno chcesz włączyć tryb sieciowy?\nUpewnij się, że druga aplikacja jest gotowa.");

        if (confirm != QMessageBox::Yes) {
            ui->checkbox_trybSieciowy->blockSignals(true);
            ui->checkbox_trybSieciowy->setChecked(false);
            ui->checkbox_trybSieciowy->blockSignals(false);
            return;
        }
    }

    bool sieciowy = (ui->checkbox_trybSieciowy->isChecked());

    if (!sieciowy)
    {

        ui->wgrajARX->setEnabled(true);
        ui->wgrajGWZ->setEnabled(true);
        ui->wgrajPID->setEnabled(true);
        ui->symuluj->setEnabled(true);
        ui->zatrzymaj->setEnabled(true);
        ui->resetuj->setEnabled(true);
        ui->zapisDoPliku->setEnabled(true);
        ui->wgrajzPliku->setEnabled(true);
        ui->reset_calka->setEnabled(true);
        ui->btn_polacz_klient->setEnabled(false);
        ui->btn_start_server->setEnabled(false);
        return;
    }

    ui->wgrajARX->setEnabled(false);
    ui->wgrajGWZ->setEnabled(false);
    ui->wgrajPID->setEnabled(false);
    ui->symuluj->setEnabled(false);
    ui->zatrzymaj->setEnabled(false);
    ui->resetuj->setEnabled(false);
    ui->zapisDoPliku->setEnabled(false);
    ui->wgrajzPliku->setEnabled(false);
    ui->reset_calka->setEnabled(false);
    ui->btn_polacz_klient->setEnabled(true);
    ui->btn_start_server->setEnabled(true);
}

void UkladAutomatycznejRegulacji::onSterowanieReceived(double sterowanie)
{
    serverTime += interwal/1000.0;


    ui->customPlot->graph(1)->addData(serverTime, sterowanie);

    if (serverTime > ui->customPlot->xAxis->range().upper) {
        ui->customPlot->xAxis->setRange(serverTime, 10, Qt::AlignRight);
    }
    rescaleVisibleY(ui->customPlot);
    ui->customPlot->replot();
}

void UkladAutomatycznejRegulacji::onWartoscZadanaReceived(double wartoscZadana)
{

    ui->customPlot_pid->graph(0)->addData(serverTime, wartoscZadana);

    if (serverTime > ui->customPlot_pid->xAxis->range().upper) {
        ui->customPlot_pid->xAxis->setRange(serverTime, 10, Qt::AlignRight);
    }
    rescaleVisibleY(ui->customPlot_pid);
    ui->customPlot_pid->replot();

}

void UkladAutomatycznejRegulacji::onRegulatedValueReceived(double regulatedValue)
{

    ui->customPlot->graph(0)->addData(serverTime, regulatedValue);

    if (serverTime > ui->customPlot->xAxis->range().upper) {
        ui->customPlot->xAxis->setRange(serverTime, 10, Qt::AlignRight);
    }

     rescaleVisibleY(ui->customPlot);
    ui->customPlot->replot();
}


void UkladAutomatycznejRegulacji::onOtrzymanoInterwal(double interwal) {
    timer->setInterval(static_cast<int>(interwal));
    this->interwal = interwal;
}
