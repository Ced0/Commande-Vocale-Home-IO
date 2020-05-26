#include "wsainit.h"

#ifdef _WIN32

WSAInit::WSAInit()
{
    WSADATA init_win32;

    int erreur = WSAStartup(MAKEWORD(2,2), &init_win32);
    if(erreur != 0)
    {
        qDebug() << "Erreur initialisation : " << erreur << " " << WSAGetLastError() << endl;
    }else{
        qDebug() << "WSA OK\n";
    }
}

WSAInit::~WSAInit()
{
    WSACleanup();
}
#endif
