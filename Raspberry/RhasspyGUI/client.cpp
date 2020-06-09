#include "client.h"

Client::Client()
{
    int erreur;

    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        output << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    erreur = setsockopt(mySocket, IPPROTO_TCP,TCP_NODELAY,(char*)& temp, sizeof(temp));

    if(erreur != 0)
        output << "Erreur option socket : " << erreur << " " << WSAGetLastError() << endl;
}

Client::Client(const char* ip,  unsigned int port)
{
    int erreur;
    #ifdef _WIN32
    WSADATA init_win32;


    erreur = WSAStartup(MAKEWORD(2,2), &init_win32);
    if(erreur != 0)
        output << "Erreur initialisation : " << erreur << " " << WSAGetLastError() << endl;

    #endif

    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        output << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    erreur = setsockopt(mySocket, IPPROTO_TCP,TCP_NODELAY,(char*)&temp, sizeof(temp));

    if(erreur != 0)
        output << "Erreur option socket : " << erreur << " " << WSAGetLastError() << endl;

    connectSocket(ip, port);
}

Client::~Client()
{
    int erreur = closesocket(mySocket);
    if(erreur != 0)
        output << "Erreur closing socket : " << erreur << " " << WSAGetLastError() << endl;
}

bool Client::connectSocket(const char* ip, unsigned int port)
{
    sock_in.sin_family = AF_INET;
    sock_in.sin_addr.s_addr = inet_addr(ip);
    sock_in.sin_port = htons(port);
    int erreur = connect(mySocket, (struct sockaddr *) &sock_in, sizeof(sock_in));
    if(erreur != 0){
        output << "Erreur connection : " << erreur << " " << WSAGetLastError() << endl;
        return false;
    }

    output << "Connection effectuer " << endl;
    output << "Client port : " << port << endl;
    return true;
}

bool Client::envoie(const char *buffer, int size)
{
    int n = send(mySocket, buffer, size, 0);
    if(n == SOCKET_ERROR)
    {
        output << "Erreur envoie TCP : " << n << " " << WSAGetLastError() << endl;
        return false;
    }

    output << "Envoie effectuer" << endl;
    return true;
}

bool Client::reception(char *buffer, int size)
{
    int n = recv(mySocket, buffer, size, 0);
    if(n == SOCKET_ERROR)
    {
        output << "Erreur reception TCP : " << n << " " << WSAGetLastError() << endl;
        return false;
    }else{
        output << "Reception effectuer " << n << endl;
        return true;
    }
}

