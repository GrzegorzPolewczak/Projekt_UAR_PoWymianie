#include "NetworkManager.h"
#include "ModelARX.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {}

NetworkManager::~NetworkManager()
{
    if (server) server->close();
    if (socket) socket->close();
}

bool NetworkManager::startServer(quint16 port)
{
    czyJestemSerwerem = true;

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    if (!server->listen(QHostAddress::Any, port))
    {
        emit connectionFailed("Błąd podczas uruchamiania serwera");
        return false;
    }

    return true;
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    czyJestemSerwerem = false;

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &NetworkManager::onSocketError);

    socket->connectToHost(host, port);
}
void NetworkManager::onNewConnection() {
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::handleReadyRead);
    emit clientConnected();
}


void NetworkManager::onConnected()
{
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::handleReadyRead);
    emit connectedToPeer();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    emit connectionFailed("Połączenie zostało zamknięte przez serwer.");

    emit connectionLost();
}

void NetworkManager::sendSterowanie(double sterowanie)
{
    if (socket && socket->isOpen()) {
        QByteArray dane;
        QDataStream stream(&dane, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_15);
        stream << sterowanie;
        socket->write(dane);
    }
}

void NetworkManager::handleReadyRead()
{
    if (!socket)
        return;

    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_5_15);

    if (czyJestemSerwerem)
    {
        double sterowanie;
        stream >> sterowanie;

        if (modelGlobalny)
        {
            double wyjscie = modelGlobalny->wykonajKrok(sterowanie);

            QByteArray odpowiedz;
            QDataStream sOut(&odpowiedz, QIODevice::WriteOnly);
            sOut.setVersion(QDataStream::Qt_5_15);
            sOut << wyjscie;
            socket->write(odpowiedz);
            socket->flush();
        }
    }
    else
    {
        double wyjscie;
        stream >> wyjscie;

        emit otrzymanoWyjscie(wyjscie);
    }
}



void NetworkManager::setModel(ModelARX* m) {
    modelGlobalny = m;
}

void NetworkManager::disconnectFromHost()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->close();
        emit connectionLost();
    }
}


