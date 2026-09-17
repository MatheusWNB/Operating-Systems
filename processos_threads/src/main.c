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

#define TAMANHO_BUFFER 5

typedef struct{
    int *buffer;
    int counter;
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
}shared_mem;

void consumer(shared_mem *ptr){
    if(ptr->counter > 0){
        printf("////////// CONSUMER //////////\n");
        for(;ptr->counter > 0; ptr->counter--){
            printf("Counter: %d\n", ptr->buffer[ptr->counter]);
            ptr->buffer[ptr->counter] = 0;
            printf("Consumer: %d\n", ptr->buffer[ptr->counter]);
        }
    }
}

void producer(shared_mem *ptr){
    if(ptr->counter == 0){
        printf("////////// PRODUCER //////////\n");
        for(;ptr->counter < 5; ptr->counter++){
            printf("counter %d\n", ptr->counter);
            ptr->buffer[ptr->counter] = ptr->counter;
            printf("Producer: %d\n", ptr->buffer[ptr->counter]);
        }
    } 
}

int main(){
    int i;
    shared_mem *ptr;

    ptr = mmap
        (NULL,
        sizeof(shared_mem),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    ptr->buffer = mmap
        (NULL,
        TAMANHO_BUFFER * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    pthread_mutexattr_init(&ptr->attr);
    pthread_mutexattr_setpshared(&ptr->attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&ptr->mutex, &ptr->attr);
    pthread_mutexattr_destroy(&ptr->attr);

    ptr->counter = 0;

    pid_t proc = fork();

    while(true){
        if(proc == 0){
            pthread_mutex_lock(&ptr->mutex);
            producer(ptr);
            pthread_mutex_unlock(&ptr->mutex);
        }

        if(proc != 0){
            pthread_mutex_lock(&ptr->mutex);
            consumer(ptr);
            if(ptr->counter == 0){
                pthread_mutex_unlock(&ptr->mutex);
                continue;
            }
            pthread_mutex_unlock(&ptr->mutex);
        }
    }

    return 0;
}