#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "funcoes.h"

#define MAXBUF 100

int main(int argc, char **argv){

        char *MsgRead = malloc (MAXBUF*sizeof(char));
        char *MsgWrite = malloc (MAXBUF*sizeof(char));
        char Ack[2];
        struct sockaddr_in6 ServerAddr, ClientAddr;
        int UdpSocket =-1, Port;
        socklen_t MsgCheck;
        int ClientAddrLen = sizeof(ClientAddr);

        Port = atoi(argv[1]);
        int Position, i, TimeMiliseconds;
        Record AthleteTime;

        UdpSocket = socket(AF_INET6, SOCK_DGRAM, 0);

        /* Foi atribuido in6addr_any ao endereço pois ele permite (por padrão) estabelecer conexoes
        de qualquer cliente IPv4 e IPv6 que especificar a porta igual ao segundo argumento(argv[2])*/

        memset(&ServerAddr, 0, sizeof(ServerAddr));
        ServerAddr.sin6_family = AF_INET6;
        ServerAddr.sin6_port   = htons(Port);
        ServerAddr.sin6_addr   = in6addr_any;

        bind(UdpSocket,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

        /*listen() permite que o servidor aceite as proximas conexoes dos clientes. A fila de
        solicitacao de conexao tem 5 como tamanho maximo*/

        listen(UdpSocket, 5);

        Ack[1] = 6;//Por convencao, na tabela Ascii 6 corresponde a ACK

        while(1){
            memset(MsgRead, 0, MAXBUF);

            /*Tenta receber qualquer datagrama UDP de entrada. O endereço e a porta de solicitação
            do cliente serão armazenados na variável ClientAddr*/

            Table *AthletesPos = malloc(sizeof(Table));

             do{
                // Le a mensagem escrita pelo cliente
                MsgCheck = recvfrom(UdpSocket,MsgRead,MAXBUF,0,(struct sockaddr *)&ClientAddr, &ClientAddrLen);

                if(MsgCheck >= 0){ //Confirma para o cliente que a msg chegou
                    Ack[0] = MsgRead[0]; //O Ack deve mostrar pra qual msg ele se refere
                    sendto(UdpSocket, &Ack, 2, 0,(struct sockaddr *)&ClientAddr, ClientAddrLen);
                }

                memmove(MsgRead, MsgRead+1, strlen(MsgRead)); //Remove o cabecalho (primeiro caracter)

                TimeMiliseconds = ChangeToMilisec(MsgRead, strlen(MsgRead));

                if(TimeMiliseconds>=0){

                    AthleteTime.Key = TimeMiliseconds;

                    InsertTable(AthleteTime, AthletesPos);

                    InsertionSort(AthletesPos->Item, AthletesPos->n);

                    Position = BinarySearch(AthleteTime.Key, AthletesPos);

                    sprintf(MsgWrite,"%d", Position);

                }else{
                    sprintf(MsgWrite,"%d", TimeMiliseconds);
                }

                sendto(UdpSocket, MsgWrite, MAXBUF, 0, (struct sockaddr *)&ClientAddr, ClientAddrLen);

            }while(TimeMiliseconds>=0);

            strcpy(MsgRead, "");
            strcpy(MsgWrite, "");
            free(AthletesPos);

        }

        close(UdpSocket);
        free(MsgRead);
        free(MsgWrite);

}
