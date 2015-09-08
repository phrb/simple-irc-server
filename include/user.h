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
    int current_channel;
    int socket;
    pthread_t thread;
    pthread_mutex_t socket_mutex;
} User;

typedef struct _Node {
    struct _Node *prev;
    struct _Node *next;
    User *payload;
} Node;

int print_node_list(Node *);
int length(Node *);
int change_channel(User *, int);
int change_name(User *, char *);

Node *add_user(Node *, char *, char *, int, int, int);
Node *remove_user(Node *, char *);
Node *empty_user_list();
User *get_user_by_id(Node *, int);
User *get_user_by_name(Node *, char *);

#endif
