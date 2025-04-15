#include "NetworkManager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {}

NetworkManager::~NetworkManager()
{
    if (server) server->close();
    if (socket) socket->close();
}

bool NetworkManager::startServer(quint16 port)
{
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
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);

    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &NetworkManager::onSocketError);

    socket->connectToHost(host, port);
}

void NetworkManager::onNewConnection()
{
    socket = server->nextPendingConnection();
    emit clientConnected();
}

void NetworkManager::onConnected()
{
    emit connectedToPeer();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    emit connectionFailed(socket->errorString());
}
