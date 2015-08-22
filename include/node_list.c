#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node {
    struct _Node *prev;
    struct _Node *next;
    int payload_type;
    char *payload[1];
} Node;

typedef struct _User {
    char name[9];
    int current_channel;
    int socket;
} User;

typedef struct _Channel {
    char name[9];
    int id;
    Node *users;
} Channel;

typedef enum {
    PAYLOAD_USER,
    PAYLOAD_CHANNEL
} Payload;

Node *add_node(Node *list, int payload_type) {
    Node *node;
    if(payload_type == PAYLOAD_USER){
        node = malloc(sizeof(Node) - 1 + sizeof(User));
    }
    else if(payload_type == PAYLOAD_CHANNEL) {
        node = malloc(sizeof(Node) - 1 + sizeof(Channel));
    }
    else {
        return NULL;
    };

    node->payload_type = payload_type;
    if(list == NULL) {
        list = node;
        list->prev = list;
        list->next = list;
    }
    else {
        node->next = list;
        node->prev = list->prev;
        list->prev = node;
        if(list->next == list) {
            list->next = node;
        }
        list = node;
    };
    return list;
};

int add_user(Node *list, char name[9], int channel, int socket) {
    if(list->payload_type != PAYLOAD_USER) {
        return 1;
    }
    else {
        User *user = &(list->payload);
        strcpy(user->name, name);
        user->current_channel = channel;
        user->socket = socket;
        return 0;
    };
};

int print_node_list(Node *list, int payload_type) {
    Node *first = list;
    Node *p = first;

    printf("Node List. Payload=%d\n", p->payload_type);
    if(p->payload_type == PAYLOAD_USER) {
        User *user = p->payload;
        printf("\tName   : %s\n", user->name);
        printf("\tChannel: %d\n", user->current_channel);
        printf("\tSocket : %d\n", user->socket);
        p = p->next;
        while(p != first) {
            user = p->payload;
            printf("\tName   : %s\n", user->name);
            printf("\tChannel: %d\n", user->current_channel);
            printf("\tSocket : %d\n", user->socket);
            p = p->next;
        };
    };
    return 0;
};

int main () {
    Node *users;

    users = add_node(users, PAYLOAD_USER);
    add_user(users, "user_00001", 1, 0);
    print_node_list(users, PAYLOAD_USER);
    users = add_node(users, PAYLOAD_USER);
    add_user(users, "user_00002", 1, 0);
    print_node_list(users, PAYLOAD_USER);
    users = add_node(users, PAYLOAD_USER);
    add_user(users, "user_00003", 1, 0);
    print_node_list(users, PAYLOAD_USER);

    return 0;
};
