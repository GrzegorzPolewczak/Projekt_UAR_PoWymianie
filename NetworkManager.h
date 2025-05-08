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

    void sendSterowanie(double sterowanie);
    void setModel(ModelARX* m);

     void disconnectFromHost();

    bool isClient() const { return socket != nullptr && socket->isOpen(); }
    bool isServer() const { return server != nullptr && server->isListening(); }


signals:
    void connectedToPeer();
    void connectionFailed(QString reason);
    void clientConnected();
    void otrzymanoWyjscie(double wyjscie);
    void connectionLost();

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

    bool czyJestemSerwerem = false;
};

#endif // NETWORKMANAGER_H
