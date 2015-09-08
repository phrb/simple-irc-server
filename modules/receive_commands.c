#include "../include/user.h"
#include "../include/util.h"
#include "../include/receive_commands.h"
#include "../include/responses.h"
#include "../include/errors.h"
#include "../include/commands.h"

void receive_nick(User *user, Node *users, char *name, char *send_line) {
    send_line = strset(":localhost ");
    if(name == NULL) {
        send_line = stradd(send_line, ERR_NONICKNAMEGIVEN);
        send_line = stradd(send_line, "\n");
    }
    else if(get_user_by_name(users, name) != NULL) {
        send_line = stradd(send_line, ERR_NICKNAMEINUSE);
        send_line = stradd(send_line, "\n");
    }
    else {
        strcpy(user->name, name);
        send_line = strset("001 ");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, "\n");
    };
    printf(">Resposta: %s", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_user(User *user, char *hostname, char *send_line) {
    send_line = strset(":localhost 001 ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "\n");
    send_line = stradd(send_line, ":localhost NOTICE * :*** Usuario <");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "> Registrado.\n");
    strcpy(user->hostname, hostname);
    printf(">Resposta: %s", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_mode(User *user, char *umode, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, MODE);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, umode);
    send_line = stradd(send_line, "\n");
    printf(">Resposta: \"%s\"", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_ping(User *user, char *send_line) {
    send_line = strset(":localhost PONG localhost :");
    send_line = stradd(send_line, strtok(NULL, " \t\r\n/"));
    send_line = stradd(send_line, "\n");
    printf(">Resposta: %s", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_whois(User *user, Node *users, char *send_line) {
    // WHOIS USER
    send_line = strset(":localhost ");
    send_line = stradd(send_line, RPL_WHOISUSER);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "_ ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ~");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " localhost * :");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "\n");
    printf(">Resposta: %s", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);

    // WHOIS SERVER
    send_line = strset(":localhost ");
    send_line = stradd(send_line, RPL_WHOISSERVER);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " localhost :Sao Paulo, BR, SA\n");

    // END WHOIS
    send_line = stradd(send_line, RPL_ENDOFWHOIS);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " :End of /WHOIS list\n");
    printf(">Resposta: %s", send_line);

    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_who(Node *users, char *query, char *send_line) {
    User *user = get_user_by_name(users, query);
    if(user != NULL) {
        // WHO
        send_line = strset(":localhost ");
        send_line = stradd(send_line, RPL_WHOREPLY);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "_ * ~");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " * :");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " localhost ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " H :0 ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "\n");
        printf(">Resposta: %s", send_line);

        pthread_mutex_lock(&user->socket_mutex);
        write(user->socket, send_line, strlen(send_line));
        pthread_mutex_unlock(&user->socket_mutex);

        // END WHO
        send_line = strset(":localhost ");
        send_line = stradd(send_line, RPL_ENDOFWHO);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " :End of /WHO list\n");
        printf(">Resposta: %s", send_line);

        pthread_mutex_lock(&user->socket_mutex);
        write(user->socket, send_line, strlen(send_line));
        pthread_mutex_unlock(&user->socket_mutex);
    };
};

void receive_quit(User *user) {
    pthread_mutex_lock(&user->socket_mutex);
    close(user->socket);
    pthread_mutex_unlock(&user->socket_mutex);
};
