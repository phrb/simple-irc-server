# Pedro Henrique Rocha Bruel, 7143336

## EP1 - MAC0448/5910 Programação para Redes de Computadores

## Um Servidor IRC

## Lista de arquivos

Os diretórios `./include` e `./modules` contêm, respectivamente,
os cabeçalhos e o código para os componentes do programa.

O diretório `./test` contém alguns testes para a lista ligada
de usuários.

### Raiz do projeto

- `./include`: Cabeçalhos;

- `./modules`: Implementações;

- `./test`: Testes;

- `LICENSE`:  GNU GPLv2;

- `Makefile`:

    * all:    Compila o projeto;
    * server: Compila `server.c`;
    * test:   Compila os testes;
    * clean:  Remove os binários.

- `server.c`: Ponto de entrada para o servidor.

### ./include e ./modules

- `commands.{h, c}`: Strings com os comandos aceitos pelo servidor.

- `connect_user.{h, c}`: 

    * Declara a lista de usuários e seu mutex lock;

    * `void connect_user(User *);`:

        Responsável pela coordenação da conexão de cada usuário.

- `errors.{h, c}`: Strings com os códigos de erro para IRC.

- `receive_commands.{h, c}`: Funções para processar os comandos do usuário.

    * `void receive_ping(User *, char *);`:

        Responde ao comando PING, com um PONG.
        
    * `void receive_user(User *, char *);`:

        Registra um usuário.
        
    * `void receive_who(Node *, char *, char *);`:

        Responde ao comando WHO, processando a lista de usuários.
        
    * `void receive_list(User *, Node *, char *);`:

        Lista os canais já disponíveis no servidor.
        
    * `void receive_part(User *, Node *, char *);`:

        Remove um usuário de seu canal.
        
    * `void receive_mode(User *, char *, char *);`:

        Responde ao comando MODE, mas não implementa diferentes modos.
        
    * `void receive_pong(User *, char *, char *);`:

        Recebe uma mensagem PONG (não implementado).
        
    * `void receive_whois(Node *, char *, char *);`:

        Responde WHOIS com as informações de um usuário.
        
    * `void receive_join(User *, Node *, char *, char *);`:

        Coloca um usuário num canal.
        
    * `void receive_nick(User *, Node *, char *, char *);`:

        Muda o nickname do usuário.
        
    * `void receive_privmsg(User *, Node *, char *, char *);`:

        Recebe e processa um comando PRIVMSG, repassando a mensagem aos
        usuários de um canal.
        
    * `Node *receive_quit(User *, Node *, pthread_mutex_t, char *);`:

        Remove o usuário do servidor, fechando a conexão pelo socket.
        
