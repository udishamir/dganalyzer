/*
 * make C99 happy
*/
#define EXIT_ERR -1
#define WORDS "/usr/share/dict/web2"
#define SOCK_PATH "/tmp/scan.sock"
#define MAX_MSG 253
#define MAX_QUEUE 5
#define DEFAULT_MSG_TEST "echo reply"
#define MAX_LEN 18
#define BENIGN 3
#define LPORT 5001
#define BDB "db/main.db"

/* functions prototype */
int levenshtein(char *s1, char *s2);
int ventry(char *domain);
int addentry(char *domain, char *v);
