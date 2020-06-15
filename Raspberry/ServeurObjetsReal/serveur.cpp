#include "serveur.h"

Serveur::Serveur(std::mutex *_mtx, QObject *parent) : QObject(parent)
{
    //threads.push_back(std::thread(&Serveur::listenerThread, this, port));
    mtx = _mtx;
}

Serveur::~Serveur()
{
    stop = true;

    //Wait for all threads to stop
    for(int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void Serveur::launchListenerThread(unsigned short port)
{
    if(threads.size() > 0)
    {
        qDebug() << "Listener thread already active !";
    }else{
        threads.push_back(std::thread(&Serveur::listenerThread, this, port));
    }

}

void Serveur::listenerThread(unsigned short port)
{
    //Create listener socket
    SOCKET socket_ecoute;//identifiant de la socket d'ecoute du cote serveur
    SOCKADDR_IN information_sur_la_source;
    //pour le thread

    //ouverture de la socket
    socket_ecoute = socket(AF_INET,SOCK_STREAM,0);

    if(socket_ecoute == INVALID_SOCKET){
        qDebug() << "desole, peux pas creer la socket du a l'erreur : " << WSAGetLastError() << endl;
        return;
    }else{
        qDebug() << "socket : OK" << endl;
    }

    //Configure socket
    int tempo = 1;
    int erreur = setsockopt(socket_ecoute, IPPROTO_TCP,TCP_NODELAY,(char*)&tempo, sizeof(tempo));

    if(erreur != 0) qDebug() << "\n desole peux pas configurer cette option du a l'erreur : " << erreur << "->" << WSAGetLastError() <<endl;
    else qDebug() << "setsockopt(): OK" << endl;

    information_sur_la_source.sin_family = AF_INET;
    information_sur_la_source.sin_addr.s_addr = INADDR_ANY;//ecoute sur toutes les IP locales
    information_sur_la_source.sin_port =htons(port);//ecoute sur le port
    erreur = bind(socket_ecoute, (struct sockaddr*)&information_sur_la_source, sizeof(information_sur_la_source));

    if(erreur != 0)
    {
        qDebug() << "\nDeolse, je ne peux pas ecouter sur ce port : " << erreur << WSAGetLastError() << endl;
        exit(1);
    }else{
        qDebug() << "bind(): OK" << endl;
    }

    //Set the socket in non-block mode
    unsigned long nonBlock = 1;

    #ifdef __linux__
    if(fcntl(socket_ecoute, F_SETFL,O_NONBLOCK))
    {
        qDebug() << "fcntl() failed with error : " << WSAGetLastError() << endl;
    }else{
        qDebug() << "fcntl()  success !" << endl;
    }
    #elif _WIN32
    if(ioctlsocket(socket_ecoute, FIONBIO, &nonBlock))
    {
        qDebug() << "ioctlsocket() failed with error : " << WSAGetLastError() << endl;
    }else{
        qDebug() << "ioctlsocket()  success !" << endl;
    }
    #endif


    //ecoute sur le socket d'ecoute
    do{
        erreur = listen (socket_ecoute, 10);
    }while(erreur != 0);

    qDebug()  << "listen(): OK" << endl;
    SOCKET socket_travail;//identifiant de la nouvelle socket client...cote serveur
//acceptation de la demande d'ouverture de session
    qDebug() << "\nAttente de la reception de damande d'ouverture de nouvelle session TCP(SYN):\n";


    tempo = sizeof(information_sur_la_source);//passer par une variable afin d'utiliser un pointeur

    while(stop == false)
    {
        #ifdef __linux__
        socket_travail = accept(socket_ecoute,(struct sockaddr*)&information_sur_la_source, (socklen_t*)&tempo);
        #elif _WIN32
        socket_travail = accept(socket_ecoute,(struct sockaddr*)&information_sur_la_source, &tempo);
        #endif
        if(socket_travail == INVALID_SOCKET){
            //qDebug() << "\nDesole, je peux pas accepter la session TCP du a l'erreur : " << WSAGetLastError() << endl;
        }else{
            qDebug() << "Nouvel socket" << endl;
            qDebug() << "accpet() : OK" << endl;
            threads.push_back(std::thread(&Serveur::ClientThread, this, socket_travail));
            emit newConnection();
        }


    }

    shutdown(socket_ecoute, 2);
    closesocket(socket_ecoute);

}

void Serveur::ClientThread(SOCKET current_client)
{
    qDebug() << "creation du thread associe a un client ..\n";
    //recupere le socket passe en tant que parametre
    char buf[300] = {0};
    //buffer pour les données a envoyer
    char sendData[10] = {0};
    int res;

    //boucle de reception des données du client
    while(stop == false)
    {
        memset(buf, 0, 300);
        res = recv(current_client, buf, sizeof(buf), 0);//reception de la commanded'un client
        //Sleep(10);
        if(res > 0)
        {
            qDebug() << buf;
        }

        if(res == -1)
        {
            //MessageBoxA(0, "erreur","erreur", MB_OK);
            qDebug() << "Erreur";
            closesocket(current_client);
            std::terminate();

        }else if(strstr(buf,"\r\n"))//commande parasite envoyee par le client
        {
            qDebug() << ".\n";

        }else if(strstr(buf, "bye"))
        {
            qDebug() << "--> commande 'quitter' recue ..\nFermeture socket.\n";//<<endl;
            strcpy(sendData, "bye bye...\r\n");
            //Sleep(10);
            send(current_client, sendData, sizeof(sendData), 0);
            //fermer le  socket associe avec ce cient et terminer le thread en cours
            closesocket(current_client);
            break;
            //std::terminate();

        }else if(strstr(buf, "temp")){
            //Answers with temperature from sensor
            mtx->lock();

            float temp = temperature();

            mtx->unlock();

            int n = send(current_client, &temp, sizeof(temp), 0);
            if(n == SOCKET_ERROR)
            {
                qDebug() << "Erreur envoie TCP : " << n << " " << WSAGetLastError() << endl;
            }



            emit received(buf, res);
        }else if(strstr(buf, "humi")){
            //Answers with humidity from sensor
            mtx->lock();

            float humi = humidity();

            mtx->unlock();

            int n = send(current_client, &humi, sizeof(humi), 0);
            if(n == SOCKET_ERROR)
            {
                qDebug() << "Erreur envoie TCP : " << n << " " << WSAGetLastError() << endl;
            }

        }else if(strstr(buf, "relay")){
            //Turn on/off the relay

            if(strstr(buf, "on")){
                relay(true);
                qDebug() << "ON";
            }else{
                relay(false);
                qDebug() << "OFF";
            }

            char ok[] = "OK";

            int n = send(current_client, ok, sizeof(ok), 0);
            if(n == SOCKET_ERROR)
            {
                qDebug() << "Erreur envoie TCP : " << n << " " << WSAGetLastError() << endl;
            }
        }
    }

    emit connectionClose();

    shutdown(current_client, 2);
    closesocket(current_client);
}

