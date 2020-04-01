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
ull reslt[WIDTH][WIDTH1];

void* count(void *arg) {
    int temp;
    ull ops = 1;
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            temp = matrixC[i][j];
            // printf("%d||", temp);
            for(int k = 1; k <= temp; k++) {
                ops*=k;
                printf("%lld||", ops);
            }
            reslt[i][j] = ops;
            //printf("%lld||", reslt[i][j]);
            ops = 1;
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

    printf("Hasil faktorial matriks 4a.c:\n");
    for (int i = 0; i < WIDTH; i++) { 
        for (int j = 0; j < WIDTH1; j++) { 
            printf("%llu ", reslt[i][j]);
        }
        printf("\n");
    }

    // shmdt(matrixC[WIDTH][WIDTH1]);
    // shmctl(shmid, IPC_RMID, NULL);
 
    return 0;
}
