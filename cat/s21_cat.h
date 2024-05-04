#include <stdio.h>
#include <string.h>
#include <getopt.h>

const struct option longopts[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {0, 0, 0, 0}
};

typedef struct {
    int b;
    int e;
    int E;
    int v;
    int n;
    int s;
    int t;
    int T;
} shortopts;

void open_close(char *filename, shortopts* flags);


void cat(shortopts* flags, FILE *f, int ch);