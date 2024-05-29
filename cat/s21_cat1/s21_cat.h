#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

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

void parser(shortopts* flags, int argc, char** argv);

void process_flag_v(shortopts* flags, int ch);

void process_flag_e(shortopts* flags, int ch, int prev);

void process_flag_t(shortopts* flags, int ch);

void cat(shortopts* flags, FILE *f);


