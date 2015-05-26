#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
    m_socket = new QTcpSocket();
    connect(m_socket, SIGNAL(connected()), this, SLOT(ST_connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(ST_disconnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(ST_readyRead()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ST_socketError(QAbstractSocket::SocketError)));

    m_dataBufferSize = 0;
}

void NetworkManager::connect(QHostAddress hostAddress, int port)
{
    m_socket->disconnectFromHost();
    m_socket->abort(); // Aborting any other connection
    m_socket->connectToHost(hostAddress, port); // Default openmode : ReadWrite
}

void NetworkManager::disconnect()
{
    m_socket->disconnectFromHost();
    emit SL_disconnected();
}

// Slots
void NetworkManager::ST_connected()
{
    emit SL_connected();
}

void NetworkManager::ST_disconnected()
{
    emit SL_disconnected();
}

void NetworkManager::ST_readyRead()
{
    QDataStream *stream(m_socket) = new QDataStream(m_socket);
    if(m_dataBufferSize == 0) {
        if(socket->bytesAvailable() < (int)sizeof(quint64)) {
            return;
        }

        *stream >> m_dataBufferSize;
    }

    // From here, we know the size of the package.
    if(socket->bytesAvailable() < m_dataBufferSize) {
        return;
    }

    //From here, we received the entire package.

    transmit(stream);
}

void NetworkManager::transmit(QDataStream *stream)
{
    emit SL_datasReceived(stream); // To the CommandListener
}

void NetworkManager::ST_socketError(QAbstractSocket::SocketError)
{

}

NetworkManager::~NetworkManager()
{
    //dtor
}
