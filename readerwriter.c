#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t smutex;
sem_t swriter;

int g = 10;
int readCount;

void *reader(void* arg){
    sem_wait(&smutex);
    readCount++;
    if (readCount == 1){
        sem_wait(&swriter);
    }
    sem_post(&smutex);
    // reading
    printf("READING VARIABLE: %d\n", g);

    sem_wait(&smutex);
    readCount--;
    if(readCount == 0){
        sem_post(&swriter);
    }
    sem_post(&smutex);
}

void *writer(void *arg){
    sem_wait(&swriter);
    printf("WRITING to %d\n", g);
    g++;
    printf("VALUE IS NOW: %d\n", g);
    sem_post(&swriter);
}


int main (int argc, char *argv[]){

    sem_init(&smutex, 0, 1);
    sem_init(&swriter, 0, 1);

    printf("Creating threads\n");

      pthread_t th[10];

    for (int i = 0; i < 10; i++){
        if (i == 5){ 
        if (pthread_create(&th[i], NULL, writer, NULL) == -1){
            return 1;
        }
        }else { 
            if (pthread_create(&th[i], NULL, reader, NULL) == -1){
            return 1;
            }
    }
    }

    for (int y = 0; y < 2; y++){
        pthread_join(th[y], NULL);
    }
    

    sem_destroy(&smutex);
    sem_destroy(&swriter);
    return 0;
}
