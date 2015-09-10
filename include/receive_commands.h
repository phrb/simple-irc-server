#ifndef RECEIVE_COMMANDS_H
#define RECEIVE_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void receive_list(User *, char *);
void receive_ping(User *, char *);
void receive_user(User *, char *);
void receive_who(Node *, char *, char *);
void receive_part(User *, Node *, char *);
void receive_mode(User *, char *, char *);
void receive_pong(User *, char *, char *);
void receive_whois(Node *, char *, char *);
void receive_join(User *, Node *, char *, char *);
void receive_nick(User *, Node *, char *, char *);
void receive_privmsg(User *, Node *, char *, char *);
Node *receive_quit(User *, Node *, pthread_mutex_t, char *);

#endif
