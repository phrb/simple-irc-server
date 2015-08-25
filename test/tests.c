#include <assert.h>
#include "../include/user.h"

int main () {
    /* user.c */
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
