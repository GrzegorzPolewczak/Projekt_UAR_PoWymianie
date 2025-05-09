#include "NetworkManager.h"
#include "ModelARX.h"
#include <QHostAddress>
#include <QAbstractSocket>
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

    if (server && server->isListening())
    {
        server->close();
        delete server; server = nullptr;
    }

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

    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::connectionLost);

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &NetworkManager::onSocketError);

    socket->connectToHost(host, port);
}
void NetworkManager::onNewConnection() {
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::handleReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::connectionLost);
    connect(socket,QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &NetworkManager::onSocketError);
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
           // qDebug() << "[SERVER] otrzymano sterowanie =" << sterowanie;
            double wyjscie = modelGlobalny->wykonajKrok(sterowanie);
            //qDebug() << "[SERVER] obliczone wyjscie =" << wyjscie;
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
        ostatnieWyjscie = wyjscie;

        //qDebug() << "[CLIENT] odebrano wyjscie =" << wyjscie;

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
        delete socket;
        socket = nullptr;
    }
}


QString NetworkManager::peerAddressString() const
{
    if (!socket)
    {
        return QString();
    }
    QHostAddress peer = socket->peerAddress();
    QString ip;

    if (peer.protocol() == QAbstractSocket::IPv6Protocol)
    {
        quint32 v4 = peer.toIPv4Address();
        if (v4 != 0)
        {
            ip = QHostAddress(v4).toString();
        }
        else
        {
            ip = peer.toString();
        }
    }
    else
    {
        ip = peer.toString();
    }

    return ip + ":" + QString::number(socket->peerPort());
}

void NetworkManager::stopServer()
{
    if (server && server->isListening())
    {
        server->close();
        delete server;
        server = nullptr;
    }
}
