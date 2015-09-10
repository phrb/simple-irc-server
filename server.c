#define _GNU_SOURCE

#include "include/connect_user.h"

int main(int argc, char **argv) {
    int server_socket, user_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    user_list = empty_user_list();
    pthread_mutex_init(&user_list_mutex, NULL);

    if(argc != 2) {
        fprintf(stderr, "Uso: %s [porta]\n", argv[0]);
        exit(1);
    };

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "[Erro chamando socket: %s\n", strerror(errno));
        exit(2);
    };

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port        = htons(atoi(argv[1]));
    if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "[Erro chamando bind: %s]\n", strerror(errno));
        exit(3);
    };

    if(listen(server_socket, LISTENQ) == -1) {
        fprintf(stderr, "[Erro chamando listen: %s]\n", strerror(errno));
        exit(4);
    };

    printf("[Servidor Inicializado na porta %s.]\n", argv[1]);
    printf("[Pressione CTRL+C para sair.]\n");

    User *new_user;
    for(;;) {
        if ((user_socket = accept(server_socket, 
                                  (struct sockaddr *) &client_address, 
                                  &client_address_size)) == -1 ) {
            fprintf(stderr, "[Erro chamando accept: %s]\n", strerror(errno));
            exit(5);
        };
        pthread_mutex_lock(&user_list_mutex);
        user_list = add_user(user_list, DUMMY_USER, inet_ntoa(client_address.sin_addr),
                             length(user_list), DUMMY_CHANNEL,
                             user_socket);
        pthread_mutex_unlock(&user_list_mutex);
        new_user = user_list->payload;

        pthread_create(&(new_user->thread), NULL, (void * (*) (void *)) connect_user, new_user);
    };
};
