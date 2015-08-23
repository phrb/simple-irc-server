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
    printf("\t\tPayload_type: %d\n", payload_type);
    node->payload_type = payload_type;
    if(list == NULL) {
        list = node;
        list->prev = list;
        list->next = list;
    }
    else {
        node->next       = list;
        node->prev       = list->prev;
        list->prev->next = node;
        list->prev       = node;
        if(list->next == list) {
            list->next = node;
        }
        list = node;
    };
    return list;
};

Node *remove_node(Node *list) {
    if(list == NULL) {
        return NULL;
    }
    else if(list->next == list) {
        free(list);
        return NULL;
    }
    else {
        Node *p = list;
        list->prev->next = list->next;
        list->next->prev = list->prev;
        list = list->next;
        free(p);
        return list;
    };

};

Node *remove_channel(Node *list, char name[9]) {
    if(list == NULL || list->payload_type != PAYLOAD_CHANNEL) {
        return NULL;
    }
    else {
        Node *p       = list;
        Channel *chan = p->payload;
        printf("Strcmp result: %d\n", strcmp(chan->name, name));
        if(strcmp(chan->name, name) == 0) {
            list = remove_node(p);
            printf("Removed: name: %s chan->name: %s\n", name, chan->name);
            return list;
        };
        p    = p->next;
        chan = p->payload;
        while(p != list) {
            printf("Strcmp result: %d\n", strcmp(chan->name, name));
            if(strcmp(chan->name, name) == 0) {
                list = remove_node(p);
                printf("Removed: name: %s chan->name: %s\n", name, chan->name);
                return list;
            };
            p    = p->next;
            chan = p->payload;
        };
        printf("Channel %s not on list.\n", name);
        return list;
    };
};

Node *add_channel(Node *list, char name[9], int id, Node* users) {
    if(list != NULL && list->payload_type != PAYLOAD_CHANNEL) {
        return list;
    }
    else if(list->payload != NULL) {
        list = add_node(list, PAYLOAD_CHANNEL);
    };
    Channel *chan = &(list->payload);
    chan->id      = id;
    chan->users   = users;
    strcpy(chan->name, name);
    return list;
};

Node *add_user(Node *list, char name[9], int channel, int socket) {
    if(list != NULL && list->payload_type != PAYLOAD_USER) {
        return list;
    }
    else {
        list = add_node(list, PAYLOAD_USER);
        User *user = &(list->payload);
        strcpy(user->name, name);
        user->current_channel = channel;
        user->socket = socket;
        return list;
    };
};

Node *remove_user(Node *list, char name[9]) {
    if(list == NULL || list->payload_type != PAYLOAD_USER) {
        return NULL;
    }
    else {
        Node *p    = list;
        User *user = p->payload;
        printf("Strcmp result: %d\n", strcmp(user->name, name));
        if(strcmp(user->name, name) == 0) {
            list = remove_node(p);
            printf("Removed: name: %s user->name: %s\n", name, user->name);
            return list;
        };
        p    = p->next;
        user = p->payload;
        while(p != list) {
            printf("Strcmp result: %d\n", strcmp(user->name, name));
            if(strcmp(user->name, name) == 0) {
                list = remove_node(p);
                printf("Removed: name: %s user->name: %s\n", name, user->name);
                return list;
            };
            p    = p->next;
            user = p->payload;
        };
        printf("User %s not on list.\n", name);
        return list;
    };
};

Node *empty_channel_list() {
    Node *chans  = malloc(sizeof(Node) - 1 + sizeof(Channel));
    chans->next         = chans;
    chans->prev         = chans;
    chans->payload_type = PAYLOAD_CHANNEL;
    chans->payload      = NULL;
    return chans;
};

int print_node_list(Node *list) {
    Node *first = list;
    Node *p = first;

    if(list == NULL) {
        printf("Empty List.\n");
        return 0;
    }
    else if(p->payload_type == PAYLOAD_USER) {
        printf("User Node List. Payload=%d\n", p->payload_type);
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
    }
    else if(p->payload_type == PAYLOAD_CHANNEL) {
        printf("Channel Node List. Payload=%d\n", p->payload_type);
        Channel *chan = p->payload;
        printf("\tName: %s\n", chan->name);
        printf("\tID  : %d\n", chan->id);
        p = p->next;
        while(p != first) {
            chan = p->payload;
            printf("\tName: %s\n", chan->name);
            printf("\tID  : %d\n", chan->id);
            p = p->next;
        };
    };
    return 0;
};

int main () {
    Node *users;
    users = add_user(users, "user_00001", 1, 0);
    print_node_list(users);
    users = add_user(users, "user_00002", 1, 0);
    print_node_list(users);
    users = add_user(users, "user_00003", 1, 0);
    print_node_list(users);

    Node *chans = empty_channel_list();
    chans = add_channel(chans, "Channel001", 0, users);
    print_node_list(chans);

    users = remove_user(users, "user_00003");
    print_node_list(users);
    users = remove_user(users, "user_00003");
    print_node_list(users);
    users = remove_user(users, "user_00001");
    print_node_list(users);
    users = remove_user(users, "user_00002");
    print_node_list(users);

    return 0;
};
