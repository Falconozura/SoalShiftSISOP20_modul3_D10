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
typedef unsigned long long int ull;

int (*matrixC)[WIDTH1];
int reslt[WIDTH][WIDTH1];

void* count(void *arg) {
    int temp;
    int ops = 0;
    int i,j,k;
    for (i = 0; i < WIDTH; i++) { 
        for (j = 0; j < WIDTH1; j++) { 
            temp = matrixC[i][j];
            // printf("%d||", temp);
            for(k = temp; k > 0; k--) {
                ops+=k;
                // printf("--%d--", ops);
            }
            reslt[i][j] = ops;
            // printf("--%d--", reslt[i][j]);
            ops = 0;
        }
    }
}

int main()
{
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[WIDTH][WIDTH1]), IPC_CREAT | 0666);
    matrixC = shmat(shmid, 0, 0);
    pthread_t thread;
    printf("Hasil dari program 4a.c:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            printf("%d ", matrixC[i][j]);
        }
        printf("\n");
    }

    pthread_create(&thread, NULL, count, NULL);
    pthread_join(thread, NULL);

    printf("Hasil operasi pernjumlahan dari matriks 4a.c:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            printf("%d ", reslt[i][j]);
        }
        printf("\n");
    }

    // shmdt(matrixC[WIDTH][WIDTH1]);
    // shmctl(shmid, IPC_RMID, NULL);
 
    return 0;
}
