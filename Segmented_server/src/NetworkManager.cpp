#include "NetworkManager.h"

using namespace std;

NetworkManager::NetworkManager(int port, QString password) : m_port(port)
{
    //Initialisations:
        m_clientList = new QList<Client *>;
        m_encryptManager = new EncryptManager(password); // Setting the password manager
            QObject::connect(m_encryptManager, SIGNAL(changePasswordDone(bool)), this, SLOT(changePasswordDone_slot(bool)));
        DemoManager *dm = new DemoManager();
        m_commandListener = new CommandListener(dm, m_encryptManager);

    //Initialisating the server
    m_server = new QTcpServer();
    out("Starting server...");
    if(m_server->listen(QHostAddress::Any, m_port)) { // The server started successfully
        connect(m_server, SIGNAL(newConnection()), this, SLOT(clientConnection()));
        Out::success("Server started successfully.");
        QString infoPort = "Server port: " + QString::number(m_port);
        QString infoIp = "Server IP: " + m_server->serverAddress().toString();
        Out::information(infoPort.toStdString());
        Out::information(infoIp.toStdString());
    } else {
        Out::failure(m_server->errorString().toStdString(), FATAL);
    }
}

void NetworkManager::clientConnection()
{
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    Client *client = new Client(clientSocket);

    connect(client, SIGNAL(readyRead(QByteArray*, Client*)), m_commandListener, SLOT(readyRead(QByteArray*, Client*)));
    connect(client, SIGNAL(disconnect(Client*)), this, SLOT(clientDisconnection(Client*)));

    // Might cause problems because of "+".
    out(QString(QString("Client connection (") + QString(clientSocket->peerAddress().toString()) + QString(((client->isIdentified()) ? ", identified)." : ", not identified yet)."))));
}

void NetworkManager::clientDisconnection(Client *from)
{
    m_clientList->removeAll(from);
}

void NetworkManager::changePasswordDone_slot(bool success)
{
    if(success) {
        //Out::success("Password successfully changed.");
    } else {
        //Out::failure("An error occured while attempting to set the new password. The previous one will be restored.");
    }
}

void NetworkManager::readyRead(QByteArray *datas, Client *from)
{
    m_commandListener->readyRead(datas, from);
}

EncryptManager *NetworkManager::getEncryptManager()
{
    return m_encryptManager;
}

void NetworkManager::wrongPassword(Client *from) {
    // Send a wrong password error, and remove the client.
    /// TODO : Write the wrong password function (Network Manager)
}

void NetworkManager::out(QString msg)
{
    cout << "\x1b[34m[Network Manager]\x1b[0m " << msg.toStdString() << endl;
}

NetworkManager::~NetworkManager()
{
    //dtor
}
