#include "../include/user.h"
#include "../include/util.h"
#include "../include/receive_commands.h"
#include "../include/responses.h"
#include "../include/errors.h"
#include "../include/commands.h"

void receive_nick(User *user, Node *users, char *name, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    if(name == NULL) {
        send_line = stradd(send_line, ERR_NONICKNAMEGIVEN);
        send_line = stradd(send_line, "\n");
    }
    else if(get_user_by_name(users, name) != NULL) {
        send_line = stradd(send_line, ERR_NICKNAMEINUSE);
        send_line = stradd(send_line, "\n");
    }
    else {
        send_line = strset(":");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "!");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, NICK);
        send_line = stradd(send_line, " :");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, "\n");
        printf(">Resposta: %s", send_line);
        user->name = strset(name);
        print_node_list(users);
        send_all(user->current_channel, send_line, users);
        return;
    };
    write(user->socket, send_line, strlen(send_line));
};

void receive_user(User *user, char *hostname, char *send_line) {
    strcpy(user->hostname, hostname);
    send_line = strset(":localhost NOTICE * :*** Bem-Vindo!\n");
    send_line = stradd(send_line, ":localhost NOTICE * :*** Procurando hostname...\n");
    if(strcmp(hostname, user->name) == 0) {
        send_line = stradd(send_line, ":localhost NOTICE * :*** Não encontrei. Registrando.\n");
    }
    else {
        send_line = stradd(send_line, ":localhost NOTICE * :*** Encontrado. Registrando.\n");
    };
    send_line = stradd(send_line, ":localhost 001 ");
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " :Bem-Vindo. Seu hostname é ");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, "\n");

    printf(">Resposta: %s", send_line);

    write(user->socket, send_line, strlen(send_line));
};

void receive_mode(User *user, char *umode, char *send_line) {
    if(umode != NULL) {
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

        write(user->socket, send_line, strlen(send_line));
    };
};

void receive_ping(User *user, char *send_line) {
    send_line = strset(":localhost PONG localhost :");
    send_line = stradd(send_line, strtok(NULL, " \t\r\n/"));
    send_line = stradd(send_line, "\n");
    printf(">Resposta: %s", send_line);

    write(user->socket, send_line, strlen(send_line));
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

    write(user->socket, send_line, strlen(send_line));

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

    write(user->socket, send_line, strlen(send_line));
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

        write(user->socket, send_line, strlen(send_line));

        // END WHO
        send_line = strset(":localhost ");
        send_line = stradd(send_line, RPL_ENDOFWHO);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " :End of /WHO list\n");
        printf(">Resposta: %s", send_line);

        write(user->socket, send_line, strlen(send_line));
    };
};

Node *receive_quit(User *user, Node *users, pthread_mutex_t list_mutex, char *send_line) {
    receive_part(user, users, send_line);

    pthread_mutex_lock(&user->socket_mutex);
    close(user->socket);
    pthread_mutex_unlock(&user->socket_mutex);

    pthread_mutex_lock(&list_mutex);
    users = remove_user(users, user->name);
    pthread_mutex_unlock(&list_mutex);

    if(length(users) > 0) {
        return users;
    }
    else {
        return empty_user_list();
    };
};

void receive_privmsg(User *user, Node *users, char *send_line, char *message) {
    char *line, *channel, *word;

    line = malloc(strlen(message) + 1);
    line = strcpy(line, message);

    // RM PRIVMSG TOKEN
    strtok(line, " \t\r\n/");
    // GET CHANNEL NAME
    channel = strtok(NULL, " #\t\r\n/");

    // BEGIN RESPONSE
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, PRIVMSG);

    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, channel);
    send_line = stradd(send_line, " :");

    // BUILD TEXT MESSAGE
    word = strtok(NULL, " :\t\r\n/");
    while (word != NULL) {
        printf("%s\n", word);
        send_line = stradd(send_line, word);
        send_line = stradd(send_line, " ");
        word = strtok(NULL, " \t\r\n/");
    };
    send_line = stradd(send_line, "\n");

    // FIND USERS
    send_others(user->name, channel, send_line, users);
};

void receive_join(User *user, Node *users, char *channel, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, JOIN);

    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, channel);
    send_line = stradd(send_line, "\n");

    user->current_channel = strset(channel);
    send_all(channel, send_line, users);
};

void receive_part(User *user, Node *users, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, PART);

    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, user->current_channel);
    send_line = stradd(send_line, "\n");

    send_all(user->current_channel, send_line, users);
    user->current_channel = strset("Default Channel");
};
