#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

# define MAX 1000

void InsertTable(Record Reg, Table *T) {
     if (T->n == MAX)
        printf("Erro : tabela cheia\n");
     else {
        T->n++;
        T->Item[T->n] = Reg;
     }
}

int BinarySearch(int x, Table *T) {

    int i, Left, Right;

    if (0 == T->n) return 0; /* vetor vazio */

    Left = 1;
    Right = T->n;

    do {

        i = (Left + Right) / 2;
        if (x > T->Item[i].Key)
            Left = i + 1; /* procura na partição direita */
        else
            Right = i - 1; /* procura na partição esquerda */

    } while ((x != T->Item[i].Key) && (Left <= Right));

    if (x == T->Item[i].Key)
        return i;
    else
        return 0;
}

void InsertionSort(Record *A, int n){

    int i, j;
    Record aux;

    for (i = 2; i <= n; i++){

        aux = A[i];
        j = i-1;
        A[0] = aux; /* sentinela */

        while (aux.Key < A[j].Key){
            A[j+1] = A[j];
            j--;
        }

        A[j+1] = aux;
    }
}

int ChangeToMilisec(char MsgRead[], int StringSize){

    /* A funcao percorre a string atribuindo o valor numerico a uma string auxiliar. Quando chega o
    caractere descrevendo o tempo (h/m/s/ms), o valor numerico e multiplicado pela quantidade correspondente
    em milissegundos. */

    int i, Idx = 0, HourToMilisec = 0, MinToMilisec = 0, Milisec = 0, SecToMilisec = 0,Time = 0;
    char *Aux = malloc(StringSize * sizeof(char));

    for (i=0; i < StringSize; i++){

            if ((MsgRead[i]!='h')&(MsgRead[i]!='m')&(MsgRead[i]!='s')) {
                Aux[Idx] = MsgRead[i];
                Idx++;
            }
            else{

                if(MsgRead[i]=='h'){
                    HourToMilisec = Time*(3600000);
                }

                else if(MsgRead[i]=='m'){
                    if(MsgRead[i+1]=='s')
                        Milisec = Time;
                    else
                        MinToMilisec = Time*(60000);
                }

                else if(MsgRead[i]=='s'){
                    if(MsgRead[i-1]!='m')
                        SecToMilisec = Time*(1000);
                }

                memset(Aux,0,strlen(Aux));
                Idx = 0;
            }

            Time = atoi(Aux);

    }

    free(Aux);

    if (Time>=0)
        return (HourToMilisec+MinToMilisec+SecToMilisec+Milisec);
    else
        return Time;
}

void ReadAndEditStr(char *Str, char NewHead, int StringSize){

    /* Le caractere por caractere da entrada ignorando o espaco, tab e \r.
    A leitura so termina quando aparece um \n. Alem disso adiciona um cabecalho
    a msg para possibilitar a confirmacao da mensagem correta por parte do servidor*/

    char Aux;
    int i=1;

    if(NewHead==123)//Primeiro caracter depois da letra z
        NewHead = 97;


    memset(Str,0,StringSize);

    Str[0] = NewHead; //Adiciona o cabecalho no inicio da msg

    while (Aux!='\n'){
        scanf("%c", &Aux);
        if((Aux!=' ')&(Aux!='\t')&(Aux!='\r')&(Aux!='\n')){
            Str[i] = Aux;
            i++;
        }
    }

}

void Delay(int Milliseconds)
{
    //Gera um delay (em milissegundos) de acordo com a variavel passada

    long Pause;
    clock_t Now,Then;

    Pause = Milliseconds*(CLOCKS_PER_SEC/1000); //Pause recebe o numero total em ms do delay
    Now = Then = clock(); //Retorna o numero de pulsos de clock

    while( (Now-Then) < Pause ) //Enquanto o intervalo de tempo nao for igual ao tempo desejado...
        Now = clock();
}

