#include "s21_grep.h"

void parser(flags *fl, int argc, char *argv[]); 
void process_file(char *filename, char *pattern, flags *fl);
void compile_reg(regex_t *regex, char *pattern, flags *fl);
void change_linebreak(char **line);



int main(int argc, char *argv[]) {
  

    flags fl = {0};
     //  НУЖНО ВИДИМО ВЫДЕЛИТЬ ПАМЯТЬ ПОД ПАТЕРН И ЕГО СЧИТАТЬ В КОМАНДНОЙ СТРОКИ В ПАТЕРН 
    //ТО ЕСТЬ ДЕЛАТЬ ЭТО ПОСЛЕ ПАРСИНГА С ОПТИНД ДО ПРОБЕЛА?  
//делать через str_cat



    //printf("str14\n");
    parser(&fl, argc, argv);
    char *pattern = malloc((strlen(argv[optind])+1) * sizeof(char));//+1 для хранения /0, чтобы функции не читали строку вне ее массива
    //printf("str16\n");
    if (pattern == NULL) {
    perror("Ошибка выделения памяти");
    exit(EXIT_FAILURE);
}
    strcpy(pattern, argv[optind]);
    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
        //printf("str18\n");
        process_file(argv[i], pattern, &fl); 
        //printf("str19\n");
    }
    

    //НАДО ДОБАВИТЬ В ШАБЛОН НАЧАЛО И КОНЕЦ СТРОКИ ^$

    return 0;

}

 void parser(flags *fl, int argc, char *argv[]){
        int opt = 0;
        //printf("str29\n");
        while(opt!= -1){
        //printf("str32\n");
            opt = getopt(argc, argv, "e:f:ivclnhso"); // : требует аргумента 
            switch(opt){
            
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




void process_file(char *filename, char *pattern, flags *fl){
    //printf("75 str\n");
    FILE *file = fopen(filename, "r");
    //printf("77 str\n");
    char *line = NULL;
    size_t length = 0; 

    if(file != NULL){
        //printf("82 str\n");
        regex_t regex; 
        compile_reg(&regex, pattern, fl); //здесь компилируется шаблон поиска для регексес
        //printf("85 str\n");
    

        while( (getline(&line, &length, file)) != -1){ //читаем построчно
        //printf("89 str\n");
            change_linebreak(&line); //заменяем ньюлайн на конец строки чтобы рег дальше работал
 
            int status = regexec(&regex, line, 1, NULL, 0); //ноль если было совпадение 
             printf("%d\n", status);

                if(!status){
                    printf("%s\n", line);
                }

        }

        //хочу сделать регекс с записью в массив совпадений в его
        //аргументах 


        regfree(&regex);
        fclose(file);
    } /*else if (!fl->s){ /// этот флаг для подавления сообщения об ошибке 
        printf("s21_grep: %s: No such file or directory\n", filename);
    }*/
}



void compile_reg(regex_t *regex, char *pattern, flags *fl){
    if(regcomp(regex, pattern, fl->i)){
        printf("PIZDA");
    }

    //добавить про флаг и про ошибку компиляции в случае не 0
    //printf("116 str\n");
}

void change_linebreak(char **line){

    int end_str = (int)strlen(*line) - 1; //индекс 
    if((*line)[end_str] == '\n'){
       (*line)[end_str] = '\0';
    }

}