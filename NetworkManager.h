#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#pragma once

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

signals:
    void connectedToPeer();
    void connectionFailed(QString reason);
    void clientConnected();

private slots:
    void onNewConnection();
    void onConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
};

#endif // NETWORKMANAGER_H
