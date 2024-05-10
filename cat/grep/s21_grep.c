#include "s21_grep.h"

void parser(flags *fl, int argc, char *argv[]); 
void process_file(char *filename, char *pattern, flags *fl);
void compile_reg(regex_t *regex, char *pattern, flags *fl);
void change_linebreak(char *line);
void c_flag(int match_count);
void cv_flag(int match_count_cv);


int main(int argc, char *argv[]) {
  

    flags fl = {0};
     


    //printf("str14\n");
    parser(&fl, argc, argv);
 
    char *pattern = malloc((strlen(argv[optind])+1) * sizeof(char));//+1 для хранения /0, чтобы функции не читали строку вне ее массива
  

    //printf("str16\n");
    if (pattern == NULL) {
    //perror("Ошибка выделения памяти");
    //exit(EXIT_FAILURE);
}
    strcpy(pattern, argv[optind]);
  
   

    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
    
        process_file(argv[i], pattern, &fl); 

    }
    

    //НАДО ДОБАВИТЬ В ШАБЛОН НАЧАЛО И КОНЕЦ СТРОКИ ^$

    return 0;

}

 void parser(flags *fl, int argc, char *argv[]){
        int opt = 0;
      
        while(opt!= -1){
      
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
  
    FILE *file = fopen(filename, "r");

    char *line = NULL;
    size_t length = 0; 
    int match_count = 0; // for flag c
    int match_count_cv = 0;

    if(file != NULL){
  
        regex_t regex; 
        compile_reg(&regex, pattern, fl); //здесь компилируется шаблон поиска для регексес
   
        
    

        while( (getline(&line, &length, file)) != -1){ //читаем построчно
         
            change_linebreak(line); //заменяем ньюлайн на конец строки чтобы рег дальше работал
     
 
            int status = regexec(&regex, line, 0, NULL, 0); //ноль если было совпадение 
          
                if(!status){
                    match_count++;
                } else {
                    match_count_cv++;
                }



                if(!status && !fl->c && !fl->v){
                    printf("%s\n", line);
                } else if (status && fl->v && !fl->c){
                    printf("%s\n", line);
                }
          

        }
        if(fl->c && !fl->v){
        c_flag(match_count);
        } else if (fl->c && fl->v){
            cv_flag(match_count_cv);
        }

        regfree(&regex);
        fclose(file);
    } /*else if (!fl->s){ /// этот флаг для подавления сообщения об ошибке 
        printf("s21_grep: %s: No such file or directory\n", filename);
    }*/
}



void compile_reg(regex_t *regex, char *pattern, flags *fl){
    if(regcomp(regex, pattern, fl->i ? REG_ICASE : REG_EXTENDED)){//если флаг i то выключаем учет регистра иначе расширенное регулярное выражение
        printf("PIZDA");
        exit(1);
    }

    //добавить про флаг i и про ошибку компиляции в случае не 0
    //reg extended
    //printf("116 str\n");
}

void change_linebreak(char *line){

    int end_str = (int)strlen(line) - 1; //индекс 
    if(line[end_str] == '\n'){
       line[end_str] = '\0';
    }

}

void c_flag(int match_count){
    printf("%d", match_count);
}

void e_flag(){

}

void cv_flag(int match_count_cv){
    printf("%d", match_count_cv);
}
//относительно работаю флаги:
// i
// c
// c + i
// v
// v + c +i 
// начат s