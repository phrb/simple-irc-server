## EP1 - MAC0448/5910 Programação para Redes de Computadores
### Pedro Henrique Rocha Bruel, 7143336

Esta é a documentação para o EP1 da disciplina MAC0448/5910.
Este projeto implementa um servidor de IRC básico, que
responde a um subconjunto dos comandos do protocolo de
Internet Relay Chat.

Este documento está organizado da seguinte forma:

1. Descrição do Servidor
2. Lista de Arquivos

## 1. Descrição do Servidor

Para compilar o projeto, use o comando:

    $ make

Para produzir o binário com os testes, use:

    $ make test

Para iniciar o servidor:

    $ ./server <porta>

Onde `<porta>` é o número da porta na qual o servidor
esperará conexões.

Os clientes `irssi` e `xchat` foram utilizados para
a compreensão das mensagens trocadas entre cliente e
servidor IRC, e este servidor é compatível com esses
dois clientes.

Para se conectar ao servidor:

    /connect localhost <porta>

O servidor implementa os seguintes comandos IRC:

* WHO:            Lista informações de um usuário;
* NICK:           Muda o nome de um usuário;
* USER:           Registra um usuário;
* LIST:           Lista os canais do servidor;
* JOIN:           Entra num canal;
* PART:           Sai de um canal;
* WHOIS:          Lista mais informações sobre um usuário;
* PRIVMSG:        Envia mensagens a um canal ou usuário;
* QUIT:           Desconecta o usuário do servidor.
* DCC:
* MACDATA:
* MACHORA:
* MACTEMPERATURA:

Além disso, o servidor implementa parcialmente alguns
comandos para interação com clientes, que garantem
a manutenção da conexão:

* PING:   Responde com um PONG;
* NOTICE: Envia notificações a um cliente.

Finalmente, o servidor possui dois canais fixos:

* WhiteHat:   "No script kiddies allowed.".
* 1337h4x0rz: ":(){ :|: & };:".

## 2. Lista de Arquivos

Os diretórios `./include` e `./modules` contêm, respectivamente,
os cabeçalhos e o código para os componentes do programa.

O diretório `./test` contém alguns testes para a lista ligada
de usuários.

Esta seção detalha os arquivos e diretórios do projeto.

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

- `responses.{h, c}`: Strings com códigos de resposta para IRC.

- `text.{h, c}`: Strings com mensagens e informações sobre o servidor.

- `user.{h, c}`: Implementação da lista de usuários:

    * `struct User`: Representa um usuário no servidor.

    * `struct Node`: Um nó da lista, duplamente ligada e circular.

    * `int length(Node *);`:

        Devolve o comprimento de uma lista de usuários.

    * `int print_node_list(Node *);`:

        Imprime uma lista de usuários no terminal.

    * `int change_name(User *, char *);`:

        Muda o nome de um usuário.

    * `int change_channel(User *, char *);`:

        Muda o canal de um usuário.

    * `Node *empty_user_list();`:

        Devolve uma lista de usuários vazia.

    * `Node *remove_user(Node *, char *);`:

        Remove um usuário da lista.

    * `User *get_user_by_id(Node *, int);`:

        Devolve o usuário com id dado, ou NULL se não existir.

    * `User *get_user_by_name(Node *, char *);`:

        Devolve o usuário com nome dado, ou NULL se não existir.

    * `char *get_users_in_channel(Node *, char *, char *);`:

        Devolve uma string com o número de usuários em dado canal.

    * `Node *add_user(Node *, char *, char *, int, char *, int);`:

        Adiciona um usuário à lista.

- `util.{h, c}`: Funções utilitárias:

    * `char *strset(char *);`:

        Devolve uma cópia da string passada como parâmetro.

    * `char *stradd(char *, char *);`:

        Devolve a concatenação de duas strings.

    * `char *uppercase(char[]);`:

        Devolve a conversão de uma string para caixa alta.

    * `void send_all(char *, char *, Node *);`:

        Envia uma mensagem a todos os usuários em um canal.

    * `void send_others(char *, char *, char *, Node *);`:

        Envia uma mensagem a todos os usuários, menos ao remetente.
