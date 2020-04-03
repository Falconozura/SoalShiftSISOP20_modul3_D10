#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char const *argv[]){
    
    FILE *fp;
    char username[100];
    char password[100];
    char c;
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[100]={0};
    char cmd[100]={0};

    while(1) {
    //create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        } 
        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
    
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
    
        //connect to server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        } 

        printf("1. Login\n2. Register\n");
        gets(cmd);
        send(sock, cmd, strlen(cmd), 0);

        if(strcmp(cmd, "Login") == 0 || strcmp(cmd, "login") == 0) {
            printf("Username: ");
            scanf("%s",username);
            printf("Password: ");
            scanf("%s",password);
            send(sock, username, strlen(username), 0);
            send(sock, password, strlen(password), 0);
            valread = read(sock, buffer, 100);
            printf("%s\n", buffer);
            memset(username, '\0', sizeof username);
            memset(password, '\0', sizeof password);
        }
        else if(strcmp(cmd, "Register") == 0 || strcmp(cmd, "register") == 0) {
            printf("Username: ");
            scanf("%s",username);
            printf("Password: ");
            scanf("%s",password);
            send(sock, username, strlen(username), 0);
            send(sock, password, strlen(password), 0);
            valread = read(sock, buffer, 100);
            printf("%s\n", buffer);
            memset(username, '\0', sizeof username);
            memset(password, '\0', sizeof password);
        }
        else if(strcmp(cmd, "exit") == 0) {
            return 0;
        }
        memset(cmd, '\0', sizeof cmd);
    }
    return 0;
}
