void receive_nick(User *user, char *name, char *send_line) {
    if(name != NULL) {
        strcpy(user->name, name);
        printf("uN: %s\n", user->name);
        send_line = strset(":localhost NOTICE * :*** Bem-vindo!\n");
        send_line = stradd(send_line, ":localhost NOTICE * :*** Voce esta no lobby.\n");
    }
    else {
        send_line = strset(":localhost ");
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, ERR_NONICKNAMEGIVEN);
        send_line = stradd(send_line, "\n");
    };
    printf(">Resposta: %s", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_user(User *user, char *send_line) {
    send_line = strset(":localhost 001 ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, ": Connected.\n");
    send_line = stradd(send_line, ":localhost NOTICE * :*** Usuario <");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "> Registrado.\n");
    printf(">Resposta: %s", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_mode(User *user, char *umode, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, MODE);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, umode);
    send_line = stradd(send_line, "\n");
    printf(">Resposta: \"%s\"", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_ping(User *user, char *send_line) {
    send_line = strset(":localhost PONG localhost :");
    send_line = stradd(send_line, strtok(NULL, " \t\r\n/"));
    send_line = stradd(send_line, "\n");
    printf(">Resposta: %s", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_whois(User *user, char *send_line) {
    send_line = strset(":localhost ");
    send_line = stradd(send_line, RPL_WHOISUSER);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, "localhost");
    send_line = stradd(send_line, "* :");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "\n");
    send_line = stradd(send_line, ":localhost ");
    send_line = stradd(send_line, RPL_ENDOFWHOIS);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " :End of /WHOIS list\n");
    printf(">Resposta: %s", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_who(User *user, char *send_line) {
    send_line = strset(":localhost ");
    send_line = stradd(send_line, RPL_WHOREPLY);
    send_line = stradd(send_line, " lobby ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " localhost ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "\n");
    send_line = stradd(send_line, ":localhost ");
    send_line = stradd(send_line, RPL_WHOREPLY);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, " :End of /WHO list\n");
    printf(">Resposta: %s", send_line);
    pthread_mutex_lock(&user->socket_mutex);
    write(user->socket, send_line, strlen(send_line));
    pthread_mutex_unlock(&user->socket_mutex);
};

void receive_quit(User *user) {
    pthread_mutex_lock(&user->socket_mutex);
    close(user->socket);
    pthread_mutex_unlock(&user->socket_mutex);
};
