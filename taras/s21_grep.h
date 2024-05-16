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
    char *patterns[10000];//указатель на массив указателей на шаблоны 
    char **input_files; //создание двумерного массива для хранения файлов, чтобы динамически управлять. хранит указатели которые
    //ссылаются на указатели отдельных строк  
    int counter_patterns;// счетчик шаблонов 
    int counter_files;
} flags;