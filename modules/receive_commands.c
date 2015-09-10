#include "../include/user.h"
#include "../include/util.h"
#include "../include/text.h"
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
        send_line = stradd(send_line, "!~");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, NICK);
        send_line = stradd(send_line, " :");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, "\n");
        user->name = strset(name);
        print_node_list(users);
        send_all(user->current_channel, send_line, users);
        return;
    };
    write(user->socket, send_line, strlen(send_line));
};

void receive_user(User *user, char *send_line) {
    send_line = stradd(send_line, WELCOME_02);
    send_line = stradd(send_line, WELCOME_03);
    send_line = stradd(send_line, WELCOME_USER_01);
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, WELCOME_USER_02);
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, "\n");

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

        write(user->socket, send_line, strlen(send_line));
    };
};

void receive_ping(User *user, char *send_line) {
    send_line = strset(DEFAULT_PONG);
    send_line = stradd(send_line, strtok(NULL, " \t\r\n/"));
    send_line = stradd(send_line, "\n");

    write(user->socket, send_line, strlen(send_line));
};

void receive_whois(Node *users, char *query, char *send_line) {
    User *user = get_user_by_name(users, query);
    if(user != NULL) {
        send_line = strset(":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_WHOISUSER);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " ~");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " * :");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "\n");

        send_line = stradd(send_line, ":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_WHOISSERVER);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, SERVER_INFO);
        send_line = stradd(send_line, "\n");

        send_line = stradd(send_line, ":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_ENDOFWHOIS);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, ENDOFWHOIS);

        write(user->socket, send_line, strlen(send_line));
    };
};

void receive_who(User *user, Node *users, char *query, char *send_line) {
    User *u = get_user_by_name(users, query);
    if(u != NULL) {
        send_line = strset(":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_WHOREPLY);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, u->name);
        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, u->current_channel);
        send_line = stradd(send_line, " ~");
        send_line = stradd(send_line, u->name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, u->hostname);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, u->name);
        send_line = stradd(send_line, " H :0 ");
        send_line = stradd(send_line, u->name);
        send_line = stradd(send_line, "\n");

        send_line = stradd(send_line, ":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_ENDOFWHO);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, u->name);
        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, u->current_channel);
        send_line = stradd(send_line, ENDOFWHO);

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

    line           = malloc(strlen(message) + 1);
    line           = strcpy(line, message);

    strtok(line, " \t\r\n/");
    channel        = strtok(NULL, " #\t\r\n/");

    send_line      = strset(":");
    send_line      = stradd(send_line, user->name);
    send_line      = stradd(send_line, "!");
    send_line      = stradd(send_line, user->hostname);
    send_line      = stradd(send_line, " ");
    send_line      = stradd(send_line, PRIVMSG);

    send_line      = stradd(send_line, " #");
    send_line      = stradd(send_line, channel);
    send_line      = stradd(send_line, " :");

    word           = strtok(NULL, " :\t\r\n/");
    if(strcmp(word, "\001DCC") == 0) {
        printf("Got to \001DCC\n");
        receive_dcc(user, users, send_line, message);
    }
    else {
        while (word   != NULL) {
            send_line  = stradd(send_line, word);
            send_line  = stradd(send_line, " ");
            word       = strtok(NULL, " \t\r\n/");
        };
        send_line      = stradd(send_line, "\n");

        send_others(user->name, channel, send_line, users);
    };
};

void receive_join(User *user, Node *users, char *channel, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "!~");
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
    user->current_channel = strset(DUMMY_CHANNEL);
};

void receive_list(User *user, Node *users, char *send_line) {
    char *chan1 = malloc(16);
    char *chan2 = malloc(16);

    chan1       = get_users_in_channel(users, CHANNELS[0], chan1);
    chan2       = get_users_in_channel(users, CHANNELS[2], chan2);

    send_line   = strset(":");
    send_line   = stradd(send_line, SERVER_NAME);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, RPL_LISTSTART);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, user->name);
    send_line   = stradd(send_line, LIST_HEADER);

    send_line   = stradd(send_line, ":");
    send_line   = stradd(send_line, SERVER_NAME);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, RPL_LIST);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, user->name);
    send_line   = stradd(send_line, " #");
    send_line   = stradd(send_line, CHANNELS[0]);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, chan1);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, CHANNELS[1]);

    send_line   = stradd(send_line, ":");
    send_line   = stradd(send_line, SERVER_NAME);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, RPL_LIST);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, user->name);
    send_line   = stradd(send_line, " #");
    send_line   = stradd(send_line, CHANNELS[2]);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, chan2);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, CHANNELS[3]);

    send_line   = stradd(send_line, ":");
    send_line   = stradd(send_line, SERVER_NAME);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, RPL_LISTEND);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, user->name);
    send_line   = stradd(send_line, " ");
    send_line   = stradd(send_line, LIST_END);

    write(user->socket, send_line, strlen(send_line));
};

void receive_macdata(User *user, char *send_line) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char date[16];

    if(strcmp(user->current_channel, DUMMY_CHANNEL) != 0) {
        send_line = strset(":");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "!");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, PRIVMSG);

        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, user->current_channel);
        send_line = stradd(send_line, " :");
    }
    else {
        send_line = strset(":irc.progredes.org NOTICE * :*** ");
    };
    sprintf(date, "%02d/%02d/%04d\n", tm.tm_mday, tm.tm_mon + 1,
                                          tm.tm_year + 1900);

    send_line     = stradd(send_line, "Data no Servidor: ");
    send_line     = stradd(send_line, date);
    send_line     = stradd(send_line, "\n");

    write(user->socket, send_line, strlen(send_line));
};

void receive_machora(User *user, char *send_line) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char date[16];

    if(strcmp(user->current_channel, DUMMY_CHANNEL) != 0) {
        send_line = strset(":");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, "!");
        send_line = stradd(send_line, user->hostname);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, PRIVMSG);

        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, user->current_channel);
        send_line = stradd(send_line, " :");
    }
    else {
        send_line = strset(":irc.progredes.org NOTICE * :*** ");
    };
    sprintf(date, "%02d:%02d:%02d -- %s\n", tm.tm_hour, tm.tm_min,
                                            tm.tm_sec, tm.tm_zone);

    send_line     = stradd(send_line, "Hora no Servidor: ");
    send_line     = stradd(send_line, date);

    write(user->socket, send_line, strlen(send_line));
};

void receive_mactemp(User *user, char *send_line) {
};

void receive_dcc(User *user, Node *users, char *send_line, char *message) {
    User *target_user;
    char *line, *target, *word;

    line           = malloc(strlen(message) + 1);
    line           = strcpy(line, message);

    strtok(line, " \t\r\n/");
    target = strtok(NULL, " \t\r\n/");
    target_user = get_user_by_name(users, target);

    send_line      = strset(":");
    send_line      = stradd(send_line, user->name);
    send_line      = stradd(send_line, "!");
    send_line      = stradd(send_line, user->hostname);
    send_line      = stradd(send_line, " ");
    send_line      = stradd(send_line, PRIVMSG);

    send_line      = stradd(send_line, " ");
    send_line      = stradd(send_line, target);
    send_line      = stradd(send_line, " :");

    word           = strtok(NULL, " :\t\r\n/");
    send_line      = stradd(send_line, "DCC ");
    word           = strtok(NULL, " \t\r\n/");
    while (word   != NULL) {
        printf("%s\n", word);
        send_line  = stradd(send_line, word);
        send_line  = stradd(send_line, " ");
        word       = strtok(NULL, " \t\r\n/\001");
    };
    send_line      = stradd(send_line, "\n");

    printf("Enviando: %s", send_line);
    write(target_user->socket, send_line, strlen(send_line));
    write(user->socket, send_line, strlen(send_line));
};
