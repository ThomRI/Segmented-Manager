#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT

    public:
        NetworkManager();
        explicit NetworkManager(QObject *parent = 0) : QObject(parent) {}
        virtual ~NetworkManager();

        void connect(QHostAddress hostAddress, int port);
        void disconnect();

    public slots: // ST = SloT
        void ST_readyRead();
        void ST_connected();
        void ST_disconnected();
        void ST_socketError(QAbstractSocket::SocketError);

    signals: // SL = SignaL
        void SL_connected();
        void SL_disconnected();
        void SL_datasReceived();

    protected:
        void transmit(QByteArray *package);
    private:
        QTcpSocket *m_socket;
        quint64 m_dataBufferSize;
};

#endif // NETWORKMANAGER_H
