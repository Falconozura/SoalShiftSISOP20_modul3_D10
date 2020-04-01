#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define LENGTH 2
#define WIDTH 4
#define WIDTH1 5
#define MAX_THREAD 4

int matrixA[WIDTH][LENGTH];
int matrixB[LENGTH][WIDTH1];
int (*matrixC)[WIDTH1] = 0;

void* multiplicationMatrix(void* arg){
    int i, j, k;
    for (i = 0 ; i < WIDTH; i++) {
        for (j = 0; j < WIDTH1; j++) {  
            for (k = 0; k < LENGTH; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
                // printf("%d||", matrixC[i][j]);
            }
            // printf("--%d--", matrixC[i][j]);
        }
    }
}
int main(){
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
    matrixC = shmat(shmid, NULL, 0);
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < LENGTH; j++) { 
            matrixA[i][j] = rand() % 20;
        } 
    }

    for (int i = 0; i < LENGTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            matrixB[i][j] = rand() % 20;
        } 
    } 

    printf("Matrix A:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < LENGTH; j++)  
            printf("%d ",matrixA[i][j]);
        printf("\n");
    } 
  
    printf("Matrix B:\n");
    for (int i = 0; i < LENGTH; i++) { 
        for (int j = 0; j < WIDTH1; j++)  
            printf("%d ",matrixB[i][j]);
        printf("\n");
    }

    pthread_t threads;

    for (int i = 0; i < WIDTH; i++) {
        for(int j = 0;j < WIDTH1; j++) {
            matrixC[i][j] = 0;
        }
        pthread_create(&threads,NULL, multiplicationMatrix, NULL);
        pthread_join(threads, NULL); 
    }    

    printf("Hasil Perkalian:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++)  
            printf("%d ",matrixC[i][j]);
        printf("\n");
    } 

    // shmdt(matrixC[WIDTH][WIDTH1]);
    // shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
