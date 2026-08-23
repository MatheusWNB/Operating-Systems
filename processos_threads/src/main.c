#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

/*
Aplicação que necessitará de comunicação entre processos e threads
Primeiro processo: Se encarrega de obter uma entrada do teclado e escrever a saída em um 
buffer ou até mesmo um arquivo de texto. Uma thread para obter os dados do teclado e 
outra para escrever os dados no buffer;

Segundo processo: Se encarrega de coletar os dados do buffer ou arquivo de texto e
então os processa da maneira necessária. Uma thread para coletar os dados do buffer e 
para processar os dados.
*/


int main(){
    pid_t proc = fork();
    pid_t pid = getpid();

    if(proc == -1){
        exit(1);
    }

    if(proc == 0){
        for(int i = 0; i < 3; i++){
            printf("Primeiro filho executando: getpid() %d | fork() %d\n", pid, proc);
            sleep(1);
        }

        exit(0);

    } else{
        printf("Processo pai: getpid() %d | fork() %d\n", pid, proc);
        waitpid(proc, NULL, 0);
        printf("Primeiro filho encerrou!\n");
        proc = fork();
    }

    if(proc == 0){
        for(int i = 0; i < 3; i++){
            printf("Segundo filho executando: getpid() %d | fork() %d\n", pid, proc);
            sleep(1);
        }

        exit(0);

    } else{
        printf("Processo pai: getpid() %d | fork() %d\n", pid, proc);
        waitpid(proc, NULL, 0);
        printf("Segundo filho encerrou!\n");
    }

    return 0;
}