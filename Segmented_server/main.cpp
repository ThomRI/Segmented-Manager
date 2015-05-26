#include <iostream>
#include <QCoreApplication>
#include "NetworkManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    NetworkManager *netM = new NetworkManager(55555, "password");

    return app.exec();
}
