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
    int id;
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

int print_node_list(Node *list) {
    Node *first = list;
    Node *p = first;

    if(list == NULL) {
        printf("Empty List.\n");
        return 0;
    }
    else if(p->payload_type == PAYLOAD_USER) {
        printf("User Node List. Payload=%d\n", p->payload_type);
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
    }
    else if(p->payload_type == PAYLOAD_CHANNEL) {
        printf("Channel Node List. Payload=%d\n", p->payload_type);
        Channel *chan = (Channel *) p->payload;
        printf("\tName: %s\n", chan->name);
        printf("\tID  : %d\n", chan->id);
        p = p->next;
        while(p != first) {
            chan = (Channel *) p->payload;
            printf("\tName: %s\n", chan->name);
            printf("\tID  : %d\n", chan->id);
            p = p->next;
        };
    };
    return 0;
};


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

int length(Node *list) {
    Node *first = list;
    Node *p = first;
    int   i = 0;

    if(list == NULL) {
        return i;
    }
    else {
        p  = p->next;
        i += 1;
        while(p != first) {
            p  = p->next;
            i += 1;
        };
    };
    return i;
};

Node *remove_channel(Node *list, char name[9]) {
    if(list == NULL || list->payload_type != PAYLOAD_CHANNEL) {
        return NULL;
    }
    else {
        Node *p       = list;
        Channel *chan = (Channel *) p->payload;
        Node *users;
        int size, i;
        if(strcmp(chan->name, name) == 0) {
            users = chan->users;
            size  = length(users);
            printf("Removing Channel. Users: %d\n", size);
            print_node_list(users);
            for(i = 0; i < size; i++) {
                users = remove_node(users->prev);
                print_node_list(users);
            };
            printf("Done.\n");
            list = remove_node(p);
            return list;
        };
        p    = p->next;
        chan = (Channel *) p->payload;
        while(p != list) {
            if(strcmp(chan->name, name) == 0) {
                users = chan->users;
                size  = length(users);
                for(i = 0; i < size; i++) {
                    users = remove_node(users);
                };
                list = remove_node(p);
                return list;
            };
            p    = p->next;
            chan = (Channel *) p->payload;
        };
        printf("Channel %s not on list.\n", name);
        return list;
    };
};

Node *add_channel(Node *list, char name[9], int id, Node* users) {
    if(list == NULL) {
        return list;
    };
    Channel *chan = (Channel *) list->payload;
    if(list->payload_type != PAYLOAD_CHANNEL) {
        return list;
    }
    else if(chan->id != -1) {
        list = add_node(list, PAYLOAD_CHANNEL);
    };
    chan       = (Channel *) list->payload;
    chan->id   = id;
    Node *p    = users;
    User *user = (User *) p->payload;
    int size   = length(users);
    int i;
    for(i = 0; i < size; i++) {
        user->current_channel = id;
        p                     = p->next;
        user                  = (User *) p->payload;
    };
    chan->users = users;
    strcpy(chan->name, name);
    return list;
};

Node *add_user(Node *list, char name[9], int id, int channel, int socket) {
    if(list == NULL) {
        return list;
    };
    User *user = (User *) list->payload;
    if(list->payload_type != PAYLOAD_USER) {
        return list;
    }
    else if (user->id != -1) {
        list = add_node(list, PAYLOAD_USER);
    };
    user                  = (User *) list->payload;
    user->id              = id;
    user->current_channel = channel;
    user->socket          = socket;
    strcpy(user->name, name);
    return list;
};

Node *remove_user(Node *list, char name[9]) {
    if(list == NULL || list->payload_type != PAYLOAD_USER) {
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

Node *empty_channel_list() {
    Node *chans         = malloc(sizeof(Node) - 1 + sizeof(Channel));
    chans->next         = chans;
    chans->prev         = chans;
    chans->payload_type = PAYLOAD_CHANNEL;
    Channel *chan       = (Channel *) chans->payload;
    chan->id            = -1;
    return chans;
};

Node *empty_user_list() {
    Node *users         = malloc(sizeof(Node) - 1 + sizeof(User));
    users->next         = users;
    users->prev         = users;
    users->payload_type = PAYLOAD_USER;
    User *user          = (User *) users->payload;
    user->id            = -1;
    return users;
};

int main () {
    Node *users = empty_user_list();
    users = add_user(users, "user_00001", 1, 1, 12121);
    print_node_list(users);
    users = add_user(users, "user_00002", 2, 1, 12312);
    print_node_list(users);
    users = add_user(users, "user_00003", 3, 1, 11122);
    print_node_list(users);

    Node *users2 = empty_user_list();
    users2 = add_user(users2, "user_00004", 4, 1, 11122);
    print_node_list(users2);
    users2 = add_user(users2, "user_00005", 5, 1, 11122);
    print_node_list(users2);

    Node *chans = empty_channel_list();
    printf("length(chans) == 1?: %d\n", length(chans));
    chans = add_channel(chans, "Channel001", 0, users);
    printf("length(chans) == 1?: %d\n", length(chans));
    print_node_list(chans);
    print_node_list(users);

    chans = add_channel(chans, "Channel002", 1, users2);
    printf("length(chans) == 2?: %d\n", length(chans));
    print_node_list(chans);
    print_node_list(users2);

    chans = remove_channel(chans, "Channel023");
    printf("length(chans) == 2?: %d\n", length(chans));
    print_node_list(chans);

    chans = remove_channel(chans, "Channel002");
    printf("length(chans) == 1?: %d\n", length(chans));
    print_node_list(chans);

    chans = remove_channel(chans, "Channel001");
    print_node_list(chans);
    chans = remove_channel(chans, "Channel001");
    print_node_list(chans);

    users = empty_user_list();
    users = add_user(users, "user_00001", 1, 1, 12121);
    print_node_list(users);
    users = add_user(users, "user_00002", 2, 1, 12312);
    print_node_list(users);
    users = add_user(users, "user_00003", 3, 1, 11122);
    print_node_list(users);
    users = remove_user(users, "user_00003");
    print_node_list(users);
    
    chans = empty_channel_list();
    printf("length(chans) == 1?: %d\n", length(chans));
    chans = add_channel(chans, "Channel004", 3, users);
    printf("length(chans) == 1?: %d\n", length(chans));
    print_node_list(chans);
    chans = remove_channel(chans, "Channel004");
    print_node_list(chans);

    chans = empty_channel_list();
    users = empty_user_list();

    chans = add_channel(chans, "Chan01", 332, users);
    Channel *chan = (Channel *) chans->payload;
    print_node_list(chan->users);
    chan->users = add_user(chan->users, "aaadrvan", 2, chan->id, 222);
    print_node_list(chan->users);
    
    return 0;
};
