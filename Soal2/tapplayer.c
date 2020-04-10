#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <wait.h>
#define PORT 8080

int main(int argc, char const *argv[]){
    
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024], cmd[1024], username[1024], password[1024], account[1024];
    int screen = 1;
    int status;
    pid_t child_id;

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

        if(screen == 1) {
            memset(buffer, 0, sizeof buffer);
            child_id = fork();
            if(child_id == 0) {
                char *argv[] = {"clear", NULL};
                execv("/usr/bin/clear", argv);
            } else
                while ((wait(&status)) > 0);
            printf("1. Login\n2. Register\nChoices: ");
            scanf("%s", cmd);
            send(sock, cmd, 1024, 0);
            if(strcmp(cmd, "Login") == 0 || strcmp(cmd, "login") == 0) {
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                sprintf(account, "%s|%s", username, password);
                send(sock, account, 1024, 0);
                recv(sock, buffer, 1025, 0);
                child_id = fork();
                if(child_id == 0) {
                    char *argv[] = {"clear", NULL};
                    execv("/usr/bin/clear", argv);
                } else
                    while ((wait(&status)) > 0);
                if(strcmp(buffer, "success") == 0) {
                    screen = 2;
                    printf("login success\n");
                } else {
                    screen = 1;
                    printf("login failed\n");
                }
                continue;
            }
            else if(strcmp(cmd, "Register") == 0 || strcmp(cmd, "register") == 0) {
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                sprintf(account, "%s|%s", username, password);
                send(sock, account, 1024, 0);
                recv(sock, buffer, 1024, 0);
                child_id = fork();
                if(strcmp(buffer, "success") == 0) {
                    screen = 1;
                    printf("register success\n");
                } else {
                    screen = 1;
                    printf("register failed\n");
                }
                continue;
            }

        }
        if (screen == 2) {
            printf("1. Find Match\n2. Logout\nChoices: ");
            scanf("%s", cmd);
            if (strcmp(cmd, "Logout") == 0 || strcmp(cmd, "logout") == 0) {
                screen = 1;
                continue;
            }
        }
        
    }
    return 0;
}
