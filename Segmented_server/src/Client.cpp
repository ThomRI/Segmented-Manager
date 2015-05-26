#include "../include/Client.h"

using namespace std;

Client::Client(QTcpSocket *clientSocket) : m_clientSocket(clientSocket)
{
    m_infos = new ClientInfos;
    m_infos->ip = m_clientSocket->peerAddress(); // Registering the ip in the client infos

    QObject::connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QObject::connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Client::dataReceived()
{
    QDataStream in(m_clientSocket); // Getting the datas

    if(m_dataSize == 0) {
        if(m_clientSocket->bytesAvailable() < (int)sizeof(m_dataSize)) {
            return;
        }

        in >> m_dataSize;
    }

    if(m_clientSocket->bytesAvailable() < m_dataSize) return;

    /// From here, we received the entire datas.
    QByteArray *datas;
    in >> *datas;

    transmit(datas);
}

void Client::disconnected()
{
    emit disconnect(this);
    cout << "Client " << m_infos->pseudo.toStdString() << " (" << m_infos->ip.toString().toStdString() << ") disconnected." << endl;
}

void Client::transmit(QByteArray *datas)
{
    // I created a function for future eventual needs. But for now, a signal is just emitted.
    emit readyRead(datas, this);
}

void Client::wrongPassword()
{
    emit wrongPassword(this); // To the NetworkManager
}

void Client::sendDatas(int commandID, QByteArray *datas)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint64) 0;
    out << commandID;
    out << *datas;
    out.device()->seek(0);
    out << ((int)sizeof(paquet) - (int)sizeof(quint64));

    quint64 size = m_clientSocket->write(paquet);
    out(QString(QString::number(size) + QString(" bytes of datas sent to client (") + QString(m_infos->pseudo) + QString(",") + QString(m_infos->ip.toString()) + QString(")"));
}

void Client::identify(ClientInfos *infos)
{
    delete(m_infos);
    m_infos = infos;
    m_infos->ip = m_clientSocket->peerAddress();
    m_infos->identified = true;

    out("Now identified !");
}

/// ~~~ GETTERS ~~~
ClientInfos* Client::getInfos()
{
    return m_infos;
}

bool Client::isIdentified()
{
    return m_infos->identified;
}

void Client::out(QString msg)
{
    std::cout << "\x1b[34m[Client (" << m_infos->pseudo.toStdString() << ")]\x1b[0m " << msg.toStdString() << std::endl;
}

Client::~Client()
{
    //dtor
}
