#ifndef CONNECT_USER_H
#define CONNECT_USER_H

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

#include "user.h"
#include "util.h"
#include "text.h"
#include "commands.h"
#include "receive_commands.h"

#define LISTENQ 1
#define MAXDATASIZE 100
#define MAXLINE 4096

Node *user_list;
pthread_mutex_t user_list_mutex;

void connect_user(User *);

#endif
