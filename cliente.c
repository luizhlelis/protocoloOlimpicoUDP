#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "funcoes.h"

#define MAXBUF 100

int main(int argc, char **argv){

    int    UdpSocket=-1, Rc, RecMsgLen;
    int WrongMsg;
    struct addrinfo Hints, *Res=NULL;
    struct sockaddr_in6 ServerAddr;
    struct timeval TimeToAnswer;
    char *MsgWrite = malloc (MAXBUF*sizeof(char));
    char *MsgRead = malloc (MAXBUF*sizeof(char));
    char Ack[2], NewHead;

    /*Cabecalho(identificador) baseado nas letras minusculas do alfabeto*/
    NewHead = 96; //Um caracter antes de "a"

    //Define tempo limite (100ms) para a confirmacao
    TimeToAnswer.tv_sec = 0;
    TimeToAnswer.tv_usec = 100000;

    memset(&Hints, 0x00, sizeof(Hints));
    Hints.ai_flags = AI_V4MAPPED;
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_DGRAM;

    Rc = inet_pton(AF_INET, argv[1], &ServerAddr);
    if (Rc == 1){ // endereco valido de IPv4?
       Hints.ai_family = AF_INET;
       Hints.ai_flags = AI_NUMERICHOST;
    }
    else{
       Rc = inet_pton(AF_INET6, argv[1], &ServerAddr);
       if (Rc == 1){ // endereco valido de IPv6?
          Hints.ai_family = AF_INET6;
          Hints.ai_flags = AI_NUMERICHOST;
       }
    }

    getaddrinfo(argv[1], argv[2], &Hints, &Res); //Pega a informacao do endereco para o servidor

    // A funcao socket() retorna um descritor de sockets que representa um endpoint.
    UdpSocket = socket(Res->ai_family, Res->ai_socktype, Res->ai_protocol);

    do{
        if(NewHead!=96){
            NewHead = MsgWrite[0]; //Copia o cabecalho da msg anterior
        }

        NewHead = NewHead+1;

        ReadAndEditStr(MsgWrite,NewHead, MAXBUF);

        sendto(UdpSocket, MsgWrite, MAXBUF, 0, Res->ai_addr, Res->ai_addrlen); //Envia Tempo do Atleta

        setsockopt(UdpSocket, SOL_SOCKET, SO_RCVTIMEO,&TimeToAnswer,sizeof(TimeToAnswer)); //Define o tempo limite para resposta do Socket

        RecMsgLen = recvfrom(UdpSocket, &Ack, 2, 0, Res->ai_addr, &Res->ai_addrlen);

        if(RecMsgLen < 0){ //Caso nao receba nenhuma msg de confirmacao
            Delay(0.1); //Aguarda o tempo necessário para a msg chegar
            sendto(UdpSocket, MsgWrite, MAXBUF, 0, Res->ai_addr, Res->ai_addrlen); //Reenvia a msg
        }

        if ((Ack[0] != NewHead)&&(Ack[1] == 6)){ //A confirmacao de uma msg antiga chegou
            WrongMsg = 1;
        }
        else if ((Ack[0] == NewHead)&&(Ack[1] == 6)){ //A confirmacao da msg atual chegou
            WrongMsg = 0;
        }

        if(WrongMsg == 0){
            recvfrom(UdpSocket, MsgRead, MAXBUF, 0,Res->ai_addr, &Res->ai_addrlen);
            if(atoi(MsgRead)>=0)
                printf("%s\n", MsgRead);
        }else{
            recvfrom(UdpSocket, MsgRead, MAXBUF, 0,Res->ai_addr, &Res->ai_addrlen);
        }

    } while(atoi(MsgRead)>=0);

    close(UdpSocket);
    freeaddrinfo(Res);
    free(MsgWrite);
    free(MsgRead);

}
