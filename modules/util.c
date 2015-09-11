#include "../include/util.h"

char *uppercase(char str[]) {
    int p = 0;
    while (str[p]) {
        str[p] = toupper(str[p]);
        p += 1;
    };
    return str;
};

char *stradd(char *str1, char *str2) {
    char *result = malloc(strlen(str1)+strlen(str2)+1);
    strcpy(result, str1);
    strcat(result, str2);
    return result;
};

char *strset(char *str) {
    char *result = malloc(strlen(str)+1);
    strcpy(result, str);
    return result;
};

void send_others(char *name, char *channel, char *message, Node *users) {
    Node *first  = users;
    Node *p      = users;
    User *target = (User *) p->payload;

    if(strcmp(target->name, name) != 0 &&
       strcmp(target->current_channel, channel) == 0) {
        write(target->socket, message, strlen(message));
    };
    p = p->next;
    target = (User *) p->payload;
    while(p != first) {
        if(strcmp(target->name, name) != 0 &&
           strcmp(target->current_channel, channel) == 0) {
            write(target->socket, message, strlen(message));
        };
        p = p->next;
        target = (User *) p->payload;
    };
};

void send_all(char *channel, char *message, Node *users) {
    Node *first  = users;
    Node *p      = users;
    User *target = (User *) p->payload;

    if(strcmp(target->current_channel, channel) == 0) {
        write(target->socket, message, strlen(message));
    };
    p = p->next;
    target = (User *) p->payload;
    while(p != first) {
        if(strcmp(target->current_channel, channel) == 0) {
            write(target->socket, message, strlen(message));
        };
        p = p->next;
        target = (User *) p->payload;
    };
};

void send_names(User *user, Node *users, char *send_line) {
    Node *first  = users;
    Node *p      = users;

    User *target = (User *) p->payload;
    send_line = strset(":");
    send_line = stradd(send_line, SERVER_NAME);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, RPL_NAMREPLY);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " @ #");
    send_line = stradd(send_line, user->current_channel);
    send_line = stradd(send_line, " :");
    if(strcmp(target->current_channel, user->current_channel) == 0) {
        send_line = stradd(send_line, target->name);
        send_line = stradd(send_line, " ");
    };
    p = p->next;
    target = (User *) p->payload;
    while(p != first) {
        if(strcmp(target->current_channel, user->current_channel) == 0) {
            send_line = stradd(send_line, target->name);
            send_line = stradd(send_line, " ");
        };
        p = p->next;
        target = (User *) p->payload;
    };
    send_line = stradd(send_line, "\n");
    send_line = stradd(send_line, ":");
    send_line = stradd(send_line, SERVER_NAME);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, RPL_ENDOFNAMES);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, user->current_channel);
    send_line = stradd(send_line, ENDOFNAMES);
    send_line = stradd(send_line, "\n");
    send_all(user->current_channel, send_line, users);
};

void send_user_info(User *user, User *target, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, SERVER_NAME);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, RPL_WHOREPLY);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, target->name);
    send_line = stradd(send_line, " ");
    if(strcmp(target->current_channel, DUMMY_CHANNEL) == 0) {
        send_line = stradd(send_line, DUMMY_CHANNEL);
    }
    else {
        send_line = stradd(send_line, "#");
        send_line = stradd(send_line, target->current_channel);
    };
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, target->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, target->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, SERVER_NAME);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, target->name);
    send_line = stradd(send_line, " H :0 ");
    send_line = stradd(send_line, target->name);
    send_line = stradd(send_line, "\n");

    write(user->socket, send_line, strlen(send_line));
};

void send_others_info(User *user, Node *users, char *send_line) {
    Node *first = users;
    Node *p     = first;
    int sent    = 0;

    User *u = (User *) p->payload;
    if(strcmp(u->name, user->name) != 0 &&
       strcmp(u->current_channel, user->current_channel) != 0) {
        send_user_info(user, u, send_line);
        sent += 1;
    };
    p = p->next;
    while(p != first) {
        u = (User *) p->payload;
        if(strcmp(u->name, user->name) != 0 &&
           strcmp(u->current_channel, user->current_channel) != 0) {
            send_user_info(user, u, send_line);
            sent += 1;
        };
        p = p->next;
    };
    if(sent > 0) {
        send_line = strset(":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_ENDOFWHO);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, user->current_channel);
        send_line = stradd(send_line, ENDOFWHO);

        write(user->socket, send_line, strlen(send_line));
    };
};

void send_channel_users_info(User *user, Node *users, char *query, char *send_line) {
    Node *first = users;
    Node *p     = first;
    int sent    = 0;

    User *u = (User *) p->payload;
    if(strcmp(u->current_channel, query) == 0) {
        send_user_info(user, u, send_line);
        sent += 1;
    };
    p = p->next;
    while(p != first) {
        u = (User *) p->payload;
        if(strcmp(u->current_channel, query) == 0) {
            send_user_info(user, u, send_line);
            sent += 1;
        };
        p = p->next;
    };
    if(sent > 0) {
        send_line = strset(":");
        send_line = stradd(send_line, SERVER_NAME);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, RPL_ENDOFWHO);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, user->name);
        send_line = stradd(send_line, " #");
        send_line = stradd(send_line, user->current_channel);
        send_line = stradd(send_line, ENDOFWHO);

        write(user->socket, send_line, strlen(send_line));
    };
};
