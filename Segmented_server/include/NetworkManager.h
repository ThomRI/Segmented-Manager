#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QCryptographicHash>
#include <QString>
#include <QList>
#include <QtNetwork>
#include "Client.h"
#include "CommandListener.h"
#include "Out.h"

class NetworkManager : public QObject, public Out
{
    Q_OBJECT

    public:
        NetworkManager(int port, QString password = "");
        explicit NetworkManager(QObject *parent = 0) : QObject(parent) {}
        virtual ~NetworkManager();

        EncryptManager *getEncryptManager();

    public slots:
        void wrongPassword(Client *from);

        // QTcpServer m_server
        void clientConnection();

        // Client
        void readyRead(QByteArray *datas, Client *from);
        void clientDisconnection(Client *from);

        // EncryptManager
        void changePasswordDone_slot(bool success);


    protected:
        void success(std::string msg);
        void failure(std::string msg, bool fatal = 0);
        void out(QString msg);

    private:
        QTcpServer *m_server;
        QList<Client *> *m_clientList;
        int m_port;

        EncryptManager *m_encryptManager;

        CommandListener *m_commandListener;


};

#endif // NETWORKMANAGER_H
