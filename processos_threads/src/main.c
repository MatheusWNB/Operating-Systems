#include "libs/libs.h"

/*
Aplicação que necessitará de comunicação entre processos e threads
Primeiro processo: Se encarrega de obter uma entrada do teclado e escrever a saída em um 
buffer ou até mesmo um arquivo de texto. Uma thread para obter os dados do teclado e 
outra para escrever os dados no buffer;

Segundo processo: Se encarrega de coletar os dados do buffer ou arquivo de texto e
então os processa da maneira necessária. Uma thread para coletar os dados do buffer e 
para processar os dados.
*/

#define TAMANHO_BUFFER 15
int main(){
    int i;
    int buffer[15];
    int *ptr = mmap
        (NULL,
        TAMANHO_BUFFER * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    for(i = 0; i < TAMANHO_BUFFER; i++){
        ptr[i] = i * 3;
        printf("Num: %d\n", ptr[i]);
    }

    return 0;
}