#include "s21_grep.h"

void parser(flags *fl); 
void process_file();
void compile_reg(regex_t,)

int main(int argc, char *argv[]) {

    flags fl = {0};
    char *pattern

    return 0;

}

 parser(flags *fl, argc, argv){

        int opt = 0;

        while(opt!= -1){
            opt = getopt(argc, argv, "e:f:ivclnhso") // : требует аргумента 
            switch(flag){
                case 'e':
                fl->e = 1;
                break;
                case 'i':
                fl->i = 1;
                break;
                case 'v':
                fl->v = 1;
                break;
                case 'c':
                fl->c = 1;
                break;
                case 'l':
                fl->l = 1;
                break;
                case 'n':
                fl->n = 1;
                break;
                case 'h':
                fl->h = 1;
                break;
                case 's':
                fl->s = 1;
                break;
                case 'f':
                fl->f = 1;
                break;
                case 'o':
                fl->o = 1;
                break;

            }
        }
    }




void process_file(char *filename, char *pattern, flags fl){
    FILE *file = fopen(filename, "r");
    if(file != NULL){
        regex_t regex; 
        compile_reg(&regex,);

//ЗДЕСЬ БУДЕМ ЧИТАТЬ 
        fclose(file);
    } else if (!fl.s){ /// этот флаг для подавления сообщения об ошибке 
        printf("s21_grep: %s: No such file or directory\n", filename);
    }
}



void reg_compile(regex_t *regex, pattern, flags fl){
    regcomp(regex, pattern, fl.i)
}