#include "CommandListener.h"

CommandListener::CommandListener()
{
    //ctor
}

void CommandListener::dataReceived(QByteArray *stream)
{
    int commandID(0);
    stream >> commandID;
}

CommandListener::~CommandListener()
{
    //dtor
}
