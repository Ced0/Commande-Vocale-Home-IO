#include "client.h"

Client::Client()
{
    int erreur;

    //Create socket
    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        output << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    //Configure socket for TCP
    erreur = setsockopt(mySocket, IPPROTO_TCP,TCP_NODELAY,(char*)& temp, sizeof(temp));

    if(erreur != 0)
        output << "Erreur option socket : " << erreur << " " << WSAGetLastError() << endl;
}

Client::Client(const char* ip,  unsigned int port)
{
    int erreur;

    //Create socket
    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        output << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    //Configure socket for TCP
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
    for(int i = 0; i < size; i++)
    {
        cout << *(buffer+i);
    }

    cout << endl;


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

