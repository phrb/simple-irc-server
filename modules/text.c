#include "../include/text.h"

char *WELCOME_01      = ":progredes.irc.org NOTICE * :*** Bem-Vindo!\n";
char *WELCOME_02      = ":progredes.irc.org NOTICE * :*** Registrando...\n";
char *WELCOME_03      = ":progredes.irc.org NOTICE * :*** Pronto!\n";
char *WELCOME_USER_01 = ":progredes.irc.org 001 ";
char *WELCOME_USER_02 = " :Bem-Vindo. Seu hostname é  ";

char *DUMMY_USER      = "unregistered_user";
char *DUMMY_CHANNEL   = "lobby";
char *DUMMY_HOST      = "unknown_host";

char *SERVER_NAME     = "progredes.irc.org";
char *SERVER_INFO     = ":São Paulo, BR, South America";
char *DEFAULT_PONG    = ":progredes.irc.org PONG progredes.irc.org :";

char *ENDOFWHOIS      = " :End of /WHOIS list\n";
char *ENDOFWHO        = " :End of /WHO list\n";
char *ENDOFNAMES      = " :End of /NAMES list\n";

char **CHANNELS       = {" Chan1 #true :1\n", " Chan2 #true :2\n"};
char *LIST_END        = " :End of /LIST\n";
char *LIST_HEADER     = " Channel :Users Name\n";
