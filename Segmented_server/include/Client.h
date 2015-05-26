#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <QtNetwork>
#include <QString>
#include <QByteArray>
#include <QTcpSocket> // Maybe in <QtNetwork> already ?
#include "Out.h"

/// The client informations (pseudo, ip, ...)
class ClientInfos {
    public:
        QString pseudo = "Unknown";
        QHostAddress ip;

        int privilegeLevel = 0;

        bool identified = false; // We can't do anything with demos until we identified.
};

class Client : public QObject, public Out
{
    Q_OBJECT

    public:
        Client(QTcpSocket *clientSocket);
        explicit Client(QObject *parent = 0) : QObject(parent) {}

        void sendDatas(int commandID, QByteArray *datas);
        void transmit(QByteArray *datas); // Used to transmit datas to the CommandListener
        void wrongPassword();

        void identify(ClientInfos *infos);


        /// ~~~ SETTERS ~~~

        /// ~~~ GETTERS ~~~
        ClientInfos *getInfos();
        bool isIdentified();

        virtual ~Client();

    public slots:
        void dataReceived();
        void disconnected();

    signals:
        void readyRead(QByteArray *datas, Client *from);
        void wrongPassword(Client *from);

        void disconnect(Client *from);
    protected:
        void out(QString msg);
    private:

        QTcpSocket *m_clientSocket;
        quint64 m_dataSize;

        ClientInfos *m_infos;

};

#endif // CLIENT_H
