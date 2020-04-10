#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

struct player {
  int jlh;
  struct sockaddr_in playerAddress;
  int addrlen;
};

struct player Player[1024];
int playerCount = 0;
pthread_t thread[1024];

void *process(void *PlayerUp) {
  struct player* playerUp = (struct player*) PlayerUp;
  int new_socket = playerUp->jlh;
  
  while(1) {
    char account[1024], secret[1024], buffer[1024], cmd[1024];
    int read = recv(new_socket, account, 1024, 0);
    account[read] = '\0';
    char cek = 0;
    memset(buffer, 0, sizeof buffer);
    if (strcmp(account, "Login") == 0 || strcmp(account, "login") == 0) {
      FILE *fp = fopen("akun.txt", "r");
      if(!fp) {
        FILE *file2 = fopen("akun.txt", "w");
        fclose(file2);
        FILE *file = fopen("akun.txt", "r");
      }
      read = recv(new_socket, account, 1024, 0);
      while (fscanf(fp, "%s", secret) != EOF) {
        if(!strcmp(secret, account)) {
          cek = 1;
          break;
        }
      }
      if(cek) {
        printf("Auth success\n");
        strcpy(cmd, "success");
        send(new_socket, cmd, 1024, 0);
      } else {
          printf("Auth failed\n");
          strcpy(cmd, "failed");
          send(new_socket, cmd, 1024, 0);
      }
      fclose(fp);
    } 
    else if (strcmp(account, "Register") == 0 || strcmp(account, "register") == 0) {
      FILE *fp = fopen("akun.txt", "a");
      read = recv(new_socket, account, 1024, 0);
      account[read] = '\0';
      fprintf(fp, "%s", account);
      fprintf(fp, "\n");
      strcpy(cmd,"success");
      fclose(fp);
      FILE *fp1 = fopen("akun.txt", "r");
      while (fscanf(fp1, "%s", secret) != EOF) {
        printf("%s\n", secret);
      }
      fclose(fp1);
      send(new_socket, cmd, 1024, 0);
    }
  }
}

int main(int argc, char const *argv[]){
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1, stock=1;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
    perror("Could not create socket\n");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
  
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed\n");
    exit(EXIT_FAILURE);
  }
    
  if (listen(server_fd, 3) < 0) {
    perror("listen failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Waiting for making connection...\n"); 
  while (1)
  {
    Player[playerCount].jlh = accept(server_fd, (struct sockaddr *)&Player[playerCount].playerAddress, &Player[playerCount].addrlen);
    pthread_create(&thread[playerCount], NULL, process, &Player[playerCount]);
    playerCount++;
  }
  for(int i = 0; i < playerCount; i++)
    pthread_join(thread[i], NULL);
  return 0;
}
