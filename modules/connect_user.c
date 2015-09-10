#include "../include/connect_user.h"

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
        command = uppercase(command);
        while(command != NULL) {
            printf("[Usuario %s enviou o comando \"%s\"]\n", user->name, command);
            if(strcmp(command, NICK) == 0) {
                receive_nick(user, user_list,
                             strtok(NULL, " \t\r\n/"),
                             send_message);
            }
            else if(strcmp(command, USER) == 0) {
                receive_user(user, send_message);
            }
            else if(strcmp(command, LIST) == 0) {
                receive_list(user, user_list, send_message);
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
            else if(strcmp(command, MACDATA) == 0) {
                receive_macdata(user, send_message);
            }
            else if(strcmp(command, MACHORA) == 0) {
                receive_machora(user, send_message);
            }
            else if(strcmp(command, MACTEMPERATURA) == 0) {
                receive_mactemp(user, send_message);
            }
            else if(strcmp(command, WHO) == 0) {
                receive_who(user, user_list, strtok(NULL, " #\t\r\n/"), send_message);
            }
            else if(strcmp(command, WHOIS) == 0) {
                receive_whois(user_list, strtok(NULL, " \t\r\n/"), send_message);
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
