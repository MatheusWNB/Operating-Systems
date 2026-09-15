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

typedef struct{
    int *buffer;
    int counter;
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
}shared_mem;

void consumer(int *buffer, int *counter){
    if(*counter > 0){
        for(;*counter >= 0; (*counter)--){
            printf("Consumer: %d\n", buffer[*counter]);
            buffer[*counter] = 0;
            printf("Consumer: %d\n", buffer[*counter]);
        }
    }
}

void producer(int *buffer, int *counter){
    if(*counter == 0){
        for(;*counter < TAMANHO_BUFFER; (*counter)++){
            printf("counter %d\n", *counter);
            buffer[*counter] = *counter + 5;
            printf("Producer: %d\n", buffer[*counter]);
        }
    }
}

int main(){
    int i;
    shared_mem *ptr;
    shared_mem shared;
    shared.counter = 0;

    pthread_mutexattr_init(&shared.attr);
    pthread_mutexattr_setpshared(&shared.attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared.mutex, &shared.attr);
    pthread_mutexattr_destroy(&shared.attr);

    ptr = mmap
        (NULL,
        sizeof(shared_mem),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    shared.buffer = mmap
        (NULL,
        TAMANHO_BUFFER * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    pid_t proc = fork();

    while(true){
        if(proc == 0){
            pthread_mutex_lock(&shared.mutex);
            producer(shared.buffer, &shared.counter);
            pthread_mutex_unlock(&shared.mutex);
        }

        if(proc != 0){
            pthread_mutex_lock(&shared.mutex);
            consumer(shared.buffer, &shared.counter);
            pthread_mutex_unlock(&shared.mutex);
        }
    }

    return 0;
}