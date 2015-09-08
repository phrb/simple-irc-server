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

void receive_who(User *, char *);
void receive_quit(User *);
void receive_user(User *, char *);
void receive_ping(User *, char *);
void receive_nick(User *, char *, char *);
void receive_join(User *, char *, char *);
void receive_mode(User *, char *, char *);
void receive_pong(User *, char *, char *);
void receive_whois(User *, char *);
void receive_privmsg(User *, char *, char *);

#endif
