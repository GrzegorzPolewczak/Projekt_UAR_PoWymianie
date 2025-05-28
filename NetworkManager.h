#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#pragma once

#include "ModelARX.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    bool startServer(quint16 port);
    void connectToServer(const QString &host, quint16 port);

    void sendSterowanie(double sterowanie, double wartoscZadana);
    void setModel(ModelARX* m);

    void disconnectFromHost();
    void stopServer();

    bool isClient() const { return socket != nullptr && socket->isOpen(); }
    bool isServer() const { return server != nullptr && server->isListening(); }


    QString peerAddressString() const;

signals:
    void connectedToPeer();
    void connectionFailed(QString reason);
    void clientConnected();
    void otrzymanoWyjscie(double wyjscie);
    void connectionLost();

    void otrzymanoSterowanie(double sterowanie);
    void otrzymanoWartoscZadana(double wartoscZadana);

private slots:
    void onNewConnection();
    void onConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

public slots:
    void handleReadyRead();

private:
    ModelARX* modelGlobalny = nullptr;
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;

    double ostatnieWyjscie = 0.0;

    bool czyJestemSerwerem = false;
};

#endif // NETWORKMANAGER_H
