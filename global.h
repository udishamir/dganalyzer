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

int levenshtein(char *s1, char *s2);
