#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


#include "../include/user.h"
#include "../include/errors.h"
#include "../include/responses.h"
#include "../include/text.h"

char *strset(char *);
char *uppercase(char[]);
char *stradd(char *, char *);
void send_all(char *, char *, Node *);
void send_names(User *, Node *, char *);
void send_user_info(User *, User *, char *);
void send_others_info(User *, Node *, char *);
void send_others(char *, char *, char *, Node *);
void send_channel_users_info(User *, Node *, char *, char *);

#endif
