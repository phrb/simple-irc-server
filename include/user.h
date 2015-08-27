#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef USER_H
#define USER_H

typedef struct _User {
    char name[9];
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
int change_name(User *, char[9]);

Node *add_user(Node *, char[9], int, int, int);
Node *remove_user(Node *, char[9]);
Node *empty_user_list();
User *get_user_by_id(Node *, int);
User *get_user_by_name(Node *, char[9]);

#endif
