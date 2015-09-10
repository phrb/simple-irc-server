#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef USER_H
#define USER_H

typedef struct _User {
    char *name;
    char *hostname;
    int id;
    char *current_channel;
    int socket;
    pthread_t thread;
    pthread_mutex_t socket_mutex;
} User;

typedef struct _Node {
    struct _Node *prev;
    struct _Node *next;
    User *payload;
} Node;

int length(Node *);
int print_node_list(Node *);
int change_name(User *, char *);
int change_channel(User *, char *);

Node *empty_user_list();
Node *remove_user(Node *, char *);
User *get_user_by_id(Node *, int);
User *get_user_by_name(Node *, char *);
char *get_users_in_channel(Node *, char *, char *);
Node *add_user(Node *, char *, char *, int, char *, int);

#endif
