#ifndef WSAINIT_H
#define WSAINIT_H

#ifdef _WIN32

    #include <winsock2.h>


#include <QDebug>

class WSAInit
{
public:
    WSAInit();
    ~WSAInit();
};

#endif
#endif // WSAINIT_H
