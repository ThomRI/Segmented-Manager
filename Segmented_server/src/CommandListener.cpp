#include "../include/CommandListener.h"

using namespace std;

CommandListener::CommandListener(DemoManager *demoManager, EncryptManager *encryptManager) \
: m_demoManager(demoManager), m_encryptManager(encryptManager)
{
    m_demoManager->init(); // Parse XML file

    // Associating each function with its CommandID :
    out("Initialisating Command Listener...");
    bool success(true);
    addCommandFunction(CommandID::IDENTIFICATION, &CommandListener::C_IDENTIFICATION, &success);
    addCommandFunction(CommandID::FULL_UPDATE_REQUEST, &CommandListener::C_FULL_UPDATE_REQUEST, &success);
    addCommandFunction(CommandID::GET_DEMO_FILE, &CommandListener::C_GET_DEMO_FILE, &success);
    addCommandFunction(CommandID::GET_DEMO_INFOS, &CommandListener::C_GET_DEMO_INFOS, &success);
    Out::success("Command Listener started successfully !");

    ///TODO : Create the ConfigManager and link it to m_privilegeRequiredById.
}

bool CommandListener::addCommandFunction(CommandID::CmdEnum ID, void(CommandListener::*function)(QDataStream*, Client*), bool *success)
{
    if(m_commandFunctions.find(ID) != m_commandFunctions.end()) { // If the ID already exists
        Out::warning(QString("Command ID collision (Command ID " + QString::number(ID) + " won't be updated) /!\\ It may not work at all !").toStdString());

        if(success != NULL) {
            if(*success == true) {
                *success = false;
            }
        }
        return false; // Return failure
    }

    m_commandFunctions[ID] = function; // Otherwise we store the function

    *success = true;
    return true; // Success !
}

void CommandListener::readyRead(QByteArray *datas, Client *from)
{
    if(from->getInfos()->identified == false) {
        return; // We can't do anything before being identified.
    }

    QDataStream stream(datas, QIODevice::ReadOnly);

    //  Determinating the CommandID (the type of command)
    int commandID(0);
    stream >> commandID;

    out(QString("Received command (ID : " + QString::number(commandID)));

    /*
        As we use a TCP connection, we assume that the datas are correctly structured for each commandID,
        and we do NOT do any verification toward that at all.

        The datas pointer passed to the command functions does not contain the command ID anymore.

        !! >> The command ID of the awnsers are the same as the request's ones. (Same client side). << !!
    */

    Out::information(QString("Executing command ID : " + QString::number(commandID)).toStdString());

    // Checking if [from] has the privilege to execute the requested command :
    if(from->getInfos()->privilegeLevel >= (*m_privilegeRequiredById)[(CommandID::CmdEnum)commandID]) {
       (this->*m_commandFunctions[(CommandID::CmdEnum)commandID])(&stream, from); // Executing function
    } else { // Otherwise, we send to [from] an error.
        C_ERROR("You do not have the privilege level require to execute that command. (Command ID : " + QString::number(commandID) + ")", from);
    }
}

/*!
 * \brief This function is used to send an error message to a client.
 * \param QString error {Error message}
 * \param Client *to {Client that the message will be sent to}
 */
void CommandListener::C_ERROR(QString error, Client *to)
{
    QByteArray *package;
    QDataStream stream(package, QIODevice::WriteOnly);

    stream << error;
    to->sendDatas(CommandID::ERROR, package);

}

/// ### COMMANDS FUNCTIONS ###

/*!
 * \brief This function is used to identify a client.
          Datas structure : {pseudo}
 * \param stream {A stream to the QByteArray that contains all the datas required to identify the client.}
 * \param from  {The client that is being identified.}
*/
void CommandListener::C_IDENTIFICATION(QDataStream *stream, Client *from)
{
    QString pseudo;
    *stream >> pseudo;

    int privilegeLevelRequest(0);
    *stream >> privilegeLevelRequest;

    QByteArray Md5Sum; // Final position in the stream, so we can directly use QByteArray to get the rest.
    *stream >> Md5Sum;

    if(m_encryptManager->checkMD5(Md5Sum)) { // If the password matches
        ClientInfos *infos = new ClientInfos;
        infos->pseudo = pseudo;

        from->identify(infos);
    } else {
        from->wrongPassword();
    }
}


/*!
 * \brief This function is called when a client send a full update request.
          The server returns to it the complete arborescence (the xml file)
          Datas structure : No data required.
 * \param from {The client that is requesting a full update.}
*/
void CommandListener::C_FULL_UPDATE_REQUEST(QDataStream *stream, Client *from)
{
    QFile *xmlFile = m_demoManager->getXmlManager()->getXmlFile();
    QByteArray *toSend = new QByteArray(xmlFile->readAll());

    from->sendDatas(CommandID::FULL_UPDATE_REQUEST, toSend);

    // REMINDER : THE CLIENT MUSTN'T MODIFY THIS FILE, SO THE CLIENT PROGRAM MUST KEEP A QBYTEARRAY IN MEMORY INSTEAD OF WRITING THE FILE !!!!!!!!
    // Otherwise, if the user modifies the file, and then asks for a missing demo, or tries to access a missing folder, the server might crash...
    /// TODO : Add some server-side securities about that.
}

/*!
 * \brief This functions is called when a client sends a request in order to receive a demo file.
          The server returns a QFile.
          Datas structure : {FOLDER_ID, DEMO_ID}

 * \param stream {A stream to the QByteArray that contains the requested demo's ID}
 * \param from {The client that is requesting the demo}
*/
void CommandListener::C_GET_DEMO_FILE(QDataStream *stream, Client *from)
{
    int folderID(0), demoID(0);
    *stream >> folderID;
    *stream >> demoID;

    Demo *demo = m_demoManager->getFolderById(folderID)->getDemo(demoID);

    QByteArray *package;
    QDataStream packageStream(package, QIODevice::WriteOnly);

    packageStream << demo->serialize(); // Sends the demo infos, already serialized. Client will unserialize using Demo::unserialize(char*) function.
    from->sendDatas(CommandID::GET_DEMO_FILE, package); // Sending the package to the client.
}

/*!
 * \brief This function is called when a client sends a request in order to receive some informations about a demo.
          The server returns a DemoInfos_s.
          Datas structure : {FOLDER_ID, DEMO_ID}

 * \param stream {A stream to a QByteArray that contains the requested demo's ID}
 * \param from {The client that is requesting the informations}
*/
void CommandListener::C_GET_DEMO_INFOS(QDataStream *stream, Client *from)
{

}

void CommandListener::out(QString msg)
{
    std::cout << "\x1b[34m[Command Listener]\x1b[0m " << msg.toStdString() << std::endl;
}

CommandListener::~CommandListener()
{
    //dtor
}
