#include "../include/user.h"

int print_node_list(Node *list) {
    Node *first = list;
    Node *p = first;

    if(list == NULL) {
        printf("Empty List.\n");
        return 0;
    };
    printf("User Node List.\n");
    User *user = (User *) p->payload;
    printf("\tName   : %s\n", user->name);
    printf("\tID     : %d\n", user->id);
    printf("\tChannel: %s\n", user->current_channel);
    printf("\tSocket : %d\n", user->socket);
    p = p->next;
    while(p != first) {
        user = (User *) p->payload;
        printf("\tName   : %s\n", user->name);
        printf("\tID     : %d\n", user->id);
        printf("\tChannel: %s\n", user->current_channel);
        printf("\tSocket : %d\n", user->socket);
        p = p->next;
    };
    return 0;
};

Node *add_node(Node *list) {
    Node *node = malloc(sizeof(Node));
    if(list == NULL) {
        list          = node;
        list->prev    = list;
        list->next    = list;
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
    list->payload = malloc(sizeof(User));
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

int length(Node *list) {
    if(list == NULL || (list->payload)->id == -1) {
        return 0;
    }
    else {
        Node *first = list;
        Node *p     = first;
        int   i     = 1;
        p           = p->next;
        while(p != first) {
            p  = p->next;
            i += 1;
        };
        return i;
    };
};

Node *add_user(Node *list, char *name, char *hostname, int id, char *channel, int socket) {
    if(list == NULL) {
        return list;
    }
    else {
        User *user            = (User *) list->payload;
        if (user->id != -1) {
            list = add_node(list);
        };
        user                  = (User *) list->payload;
        user->id              = id;
        user->socket          = socket;
        user->name            = malloc(strlen(name) + 1);
        user->current_channel = malloc(strlen(channel) + 1);
        user->hostname        = malloc(strlen(hostname) + 1);
        strcpy(user->name, name);
        strcpy(user->hostname, hostname);
        strcpy(user->current_channel, channel);
        return list;
    };
};

Node *remove_user(Node *list, char *name) {
    if(list == NULL) {
        return NULL;
    }
    else {
        Node *p    = list;
        User *user = (User *) p->payload;
        if(strcmp(user->name, name) == 0) {
            list = remove_node(p);
            return list;
        };
        p    = p->next;
        user = (User *) p->payload;
        while(p != list) {
            if(strcmp(user->name, name) == 0) {
                list = remove_node(p);
                return list;
            };
            p    = p->next;
            user = (User *) p->payload;
        };
        printf("User %s not on list.\n", name);
        return list;
    };
};

Node *empty_user_list() {
    Node *users         = malloc(sizeof(Node));
    users->next         = users;
    users->prev         = users;
    users->payload      = malloc(sizeof(User));
    User *user          = (User *) users->payload;
    user->id            = -1;
    return users;
};

User *get_user_by_id(Node *users, int id){
    int i;
    int list_length = length(users);
    Node *p         = users;
    User *user      = p->payload;
    for(i = 0; i < list_length; i++) {
        if(user->id == id) {
            return user;
        };
        p    = p->next;
        user = p->payload;
    };
    return NULL;
};

User *get_user_by_name(Node *users, char *name) {
    int i;
    int list_length = length(users);
    Node *p         = users;
    User *user      = p->payload;
    for(i = 0; i < list_length; i++) {
        if(strcmp(user->name, name) == 0) {
            return user;
        };
        p    = p->next;
        user = p->payload;
    };
    return NULL;
};

User *get_user_by_channel(Node *users, char *channel) {
    int i;
    int list_length = length(users);
    Node *p         = users;
    User *user      = p->payload;
    for(i = 0; i < list_length; i++) {
        if(strcmp(user->current_channel, channel) == 0) {
            return user;
        };
        p    = p->next;
        user = p->payload;
    };
    return NULL;
};

int change_channel(User *user, char *new_channel) {
    strcpy(user->current_channel, new_channel);
    return 0;
};

int change_name(User *user, char new_name[9]) {
    strcpy(user->name, new_name);
    return 0;
};
