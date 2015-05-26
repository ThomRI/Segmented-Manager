#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include "Client.h"
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "DemoManager.h"
#include "EncryptManager.h"
#include "Out.h"
#include <map>

struct CommandID {
    enum CmdEnum {
        ERROR,
        IDENTIFICATION,
        FULL_UPDATE_REQUEST,

        GET_DEMO_FILE,
        GET_DEMO_INFOS,
    };
};

class CommandListener: public QObject, public Out
{
    Q_OBJECT

    public:
        CommandListener(DemoManager *demoManager, EncryptManager *encryptManager);
        explicit CommandListener(QObject *parent = 0) : QObject(parent) {}
        virtual ~CommandListener();

        bool addCommandFunction(CommandID::CmdEnum ID, void(CommandListener::*function)(QDataStream*, Client*), bool *success = NULL); // Return : false(fail)/true(success)
    public slots:
        void readyRead(QByteArray *datas, Client *from); // Signal : Client::readyRead(QByteArray*, Client*)
    protected:
        void out(QString msg);

        // Commands functions
        void C_IDENTIFICATION(QDataStream *stream, Client *from); // ID : IDENTIFICATION
        void C_FULL_UPDATE_REQUEST(QDataStream *stream, Client *from); // ID : FULL_UPDATE_REQUEST
        void C_GET_DEMO_FILE(QDataStream *stream, Client *from); // ID : GET_DEMO_FILE
        void C_GET_DEMO_INFOS(QDataStream *stream, Client *from); // ID : GET_DEMO_INFOS

        void C_ERROR(QString error, Client *to);

    private:
        DemoManager *m_demoManager;
        EncryptManager *m_encryptManager;

        std::map<CommandID::CmdEnum, void (CommandListener::*)(QDataStream*, Client*)> m_commandFunctions; // Store commands functions pointers by ID
        std::map<CommandID::CmdEnum, int> *m_privilegeRequiredById; // map<int ID, int Privilege>

};

#endif // COMMANDLISTENER_H
