#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

struct user{
  char username[100];
  char password[100];
}*pUser;

int main(int argc, char const *argv[]){
  FILE *fp;
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1, stock=1;
  int addrlen = sizeof(address);
  char buffer[100] = {0};
  const char *cmd1 = "login";
  const char *cmd11 = "Login";
  const char *cmd2 = "regiser";
  const char *cmd22 = "Register";
  const char *cmd0 = "exit";
  char message[100]={0};
  char usr[100]={0};
  char pass[100]={0};

  pUser=(struct user *)malloc(sizeof(struct user));
    
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
    perror("Could not create socket\n");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt\n");
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

  while (1)
  {
    printf("Waiting for making connection...\n"); 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
      perror("accept error");
      exit(EXIT_FAILURE);
    }
    else printf("Connection accepted\n");

    valread = read(new_socket, buffer, 100);
    recv(new_socket, buffer, 100, 0);
    recv(new_socket, usr, 100, 0);
    recv(new_socket, pass, 100, 0);
    printf("%s\n", buffer);
    printf("%s\n", usr);
    printf("%s\n", pass);

    if (strcmp(buffer,cmd1) == 0 || strcmp(buffer,cmd11) == 0) {
      if ((fp=fopen("akun.txt", "r+")) == NULL) {
        if ((fp=fopen("akun.txt", "w+")) == NULL) {
          printf ("Could not open file\n");
          exit ( 1);
        }
      }
      while ( fread (pUser, sizeof(struct user), 1, fp) == 1) {
        if( strcmp ( pUser->username, usr) == 0) {
          if( strcmp ( pUser->password, pass) == 0) {
            strcpy(message,"login success\n");
            send(new_socket, message, strlen(message), 0);
            printf("Auth success\n");
          } else {
              strcpy(message,"login failed\n");
              send(new_socket, message, strlen(message), 0);
              printf("Auth failed\n");
          }
        }
      }
    }

    else if (strcmp(buffer,cmd2) == 0 || strcmp(buffer,cmd22) == 0) {
      if ((fp=fopen("akun.txt", "a+")) == NULL) {
        if ((fp=fopen("akun.txt", "w+")) == NULL) {
          printf ("Couldn't not open file\n");
          exit ( 1);
        }
      }
      fwrite (pUser->username, sizeof(struct user), 1, fp);
      fwrite (pUser->password, sizeof(struct user), 1, fp);
      printf("%s || ", pUser->username);
      printf("%s\n", pUser->password);
    }
    else if(strcmp(buffer,cmd0) == 0) {
      return 0;
    }

    memset(buffer, '\0', sizeof buffer);
    memset(message, '\0', sizeof message);
    
    close(new_socket);
  }
  return 0;
}
