#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct _User {
    char name[9];
    int id;
    int current_channel;
    int socket;
} User;

typedef struct _Node {
    struct _Node *prev;
    struct _Node *next;
    User *payload;
} Node;

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
    printf("\tChannel: %d\n", user->current_channel);
    printf("\tSocket : %d\n", user->socket);
    p = p->next;
    while(p != first) {
        user = (User *) p->payload;
        printf("\tName   : %s\n", user->name);
        printf("\tID     : %d\n", user->id);
        printf("\tChannel: %d\n", user->current_channel);
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
    if(list == NULL) {
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

Node *add_user(Node *list, char name[9], int id, int channel, int socket) {
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
        user->current_channel = channel;
        user->socket          = socket;
        strcpy(user->name, name);
        return list;
    };
};

Node *remove_user(Node *list, char name[9]) {
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

User *get_user_by_name(Node *users, char name[9]) {
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

int change_channel(User *user, int new_channel) {
    user->current_channel = new_channel;
    return 0;
};

int change_name(User *user, char new_name[9]) {
    strcpy(user->name, new_name);
    return 0;
};

int main () {
    /* Tests */
    User *u;
    Node *users = empty_user_list();

    u = users->payload;
    assert(u                             != NULL);
    assert(u->id                         == -1);

    users = add_user(users, "user_00001", 1, 1, 12121);

    u = users->payload;
    assert(strcmp(u->name, "user_00001") == 0);
    assert(u->id                         == 1);
    assert(u->current_channel            == 1);
    assert(u->socket                     == 12121);

    users = add_user(users, "user_00002", 2, 1, 12312);

    u = users->payload;
    assert(strcmp(u->name, "user_00002") == 0);
    assert(u->id                         == 2);
    assert(u->current_channel            == 1);
    assert(u->socket                     == 12312);

    users = add_user(users, "user_00003", 3, 1, 11122);

    u = users->payload;
    assert(strcmp(u->name, "user_00003") == 0);
    assert(u->id                         == 3);
    assert(u->current_channel            == 1);
    assert(u->socket                     == 11122);

    change_channel(users->payload, 3);

    u = users->payload;
    assert(u->current_channel            == 3);

    change_name(users->payload, "new_user");
    assert(strcmp(u->name, "new_user")   == 0);

    u = get_user_by_id(users, 2);
    assert(strcmp(u->name, "user_00002") == 0);

    u = get_user_by_name(users, "user_00002");
    change_channel(u, 666);
    assert(u->current_channel            == 666);
    print_node_list(users);

    return 0;
};
