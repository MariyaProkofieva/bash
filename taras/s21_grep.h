#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    int i;
    int e;
    int c;
    int v;
    int n;
    int l;
    int h;
    int s;
    int f;
    int o;
    char *patterns[10000];
    char **input_files;
    int counter_patterns;
} flags;