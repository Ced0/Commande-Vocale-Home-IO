#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

#include <iostream>

#include <QDebug>

#ifdef __linux__
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/un.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <sys/ioctl.h>
    #include <netinet/tcp.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/socket.h>

    #define SOCKET int
    #define SOCKADDR_IN sockaddr_in
    #define INVALID_SOCKET -1
    #define WSAGetLastError() errno
    #define closesocket(s) close(s)
    #define SOCKET_ERROR -1

#elif _WIN32
    #include <windows.h>
    #include <winsock2.h>
#endif

#include "grove.h"

using namespace std;

class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(std::mutex *_mtx, QObject *parent = 0);

    ~Serveur();

    void listenerThread(unsigned short port);

    void ClientThread(SOCKET current_client);

public slots:
        void launchListenerThread(unsigned short port);

signals:
    void newConnection();
    void connectionClose();

    void received(char* buff, int len);

private:
    std::vector<unsigned long long> adresse;
    vector<std::thread> threads;
    std::mutex *mtx;

    bool stop = false;
};

#endif // SERVEUR_H
