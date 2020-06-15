#include <QCoreApplication>
#include "wsainit.h"
#include "serveur.h"
#include "grove.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::mutex mtx;

    init();

    Serveur serv(&mtx);

    #ifdef _WIN32
    WSAInit wsa;
    #endif

    serv.launchListenerThread(1234);


    return a.exec();
}
