#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "include/user.h"
#include "include/util.h"
#include "include/commands.h"
#include "include/receive_commands.h"

#define LISTENQ 1
#define MAXDATASIZE 100
#define MAXLINE 4096

Node *user_list;
pthread_mutex_t user_list_mutex;

void connect_user(User *);

int main(int argc, char **argv) {
    int server_socket, user_socket;
    struct sockaddr_in server_address;

    user_list = empty_user_list();
    pthread_mutex_init(&user_list_mutex, NULL);

    if(argc != 2) {
        fprintf(stderr, "Uso: %s [porta]\n", argv[0]);
        exit(1);
    };

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "[Erro chamando socket: %s\n", strerror(errno));
        exit(2);
    };

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port        = htons(atoi(argv[1]));
    if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "[Erro chamando bind: %s]\n", strerror(errno));
        exit(3);
    };

    if(listen(server_socket, LISTENQ) == -1) {
        fprintf(stderr, "[Erro chamando listen: %s]\n", strerror(errno));
        exit(4);
    };

    printf("[Servidor Inicializado na porta %s.]\n", argv[1]);
    printf("[Pressione CTRL+C para sair.]\n");

    User *new_user;
    for(;;) {
        if ((user_socket = accept(server_socket, (struct sockaddr *) NULL, NULL)) == -1 ) {
            fprintf(stderr, "[Erro chamando accept: %s]\n", strerror(errno));
            exit(5);
        };
        pthread_mutex_lock(&user_list_mutex);
        user_list = add_user(user_list, "new_user", "new_host",
                             length(user_list), "Programacao em Redes",
                             user_socket);
        pthread_mutex_unlock(&user_list_mutex);
        new_user = user_list->payload;

        pthread_create(&(new_user->thread), NULL, (void * (*) (void *)) connect_user, new_user);
    };
};

void connect_user(User *user) {
    printf("[Usuario %d conectou-se ao servidor, esperando mensagens]\n", user->id);
    char recvline[MAXLINE + 1];
    char *line = malloc(MAXLINE + 1);
    char *send_message = malloc(MAXLINE + 1);
    char *command;
    ssize_t n;

    pthread_mutex_lock(&user->socket_mutex);
    n = read(user->socket, recvline, MAXLINE);
    pthread_mutex_unlock(&user->socket_mutex);

    while(n > 0) {
        recvline[n] = 0;
        line = strcpy(line, recvline);
        command = strtok(line, " \t\r\n/");
        while(command != NULL) {
            printf("[Usuario %d enviou \"%s\"]\n", user->id, command);
            if(strcmp(command, NICK) == 0) {
                receive_nick(user, user_list,
                             strtok(NULL, " \t\r\n/"),
                             send_message);
            }
            else if(strcmp(command, USER) == 0) {
                strtok(NULL, " \t\r\n/");
                receive_user(user, strtok(NULL, " \t\r\n/"), send_message);
            }
            else if(strcmp(command, JOIN) == 0) {
                receive_join(user, user_list, strtok(NULL, " #\t\r\n/"), send_message);
            }
            else if(strcmp(command, PART) == 0) {
                receive_part(user, user_list, send_message);
            }
            else if(strcmp(command, PRIVMSG) == 0) {
                receive_privmsg(user, user_list, send_message, recvline);
            }
            else if(strcmp(command, PING) == 0) {
                receive_ping(user, send_message);
            }
            else if(strcmp(command, WHO) == 0) {
                receive_who(user_list, strtok(NULL, " \t\r\n/"), send_message);
            }
            else if(strcmp(command, WHOIS) == 0) {
                receive_whois(user, user_list,
                              send_message);
            }
            else if(strcmp(command, MODE) == 0) {
                strtok(NULL, " \t\r\n/");
                receive_mode(user, strtok(NULL, " \t\r\n/"),
                             send_message);
            }
            else if(strcmp(command, QUIT) == 0) {
                user_list = receive_quit(user, user_list, user_list_mutex, send_message);
                return;
            };
            command = strtok(NULL, " \t\r\n/");
        };

        pthread_mutex_lock(&user->socket_mutex);
        n = read(user->socket, recvline, MAXLINE);
        pthread_mutex_unlock(&user->socket_mutex);
    };
};
