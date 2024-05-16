#include "s21_grep.h"

void parser(flags *fl, int argc, char *argv[]); 
void process_file(char *filename, char **pattern, flags fl);
void compile_reg(regex_t *regex, char **pattern, flags fl);
void change_linebreak(char *line);
void c_flag(int match_count);
void cv_flag(int match_count_cv);
void add_template_file(flags *fl, char *template_file);

//ЧТО ИСПРАВИТЬ? НАДО, ЧТОБЫ ПОСЛЕ ФЛАГА СРАЗУ ЖЕ ОБРАБАТЫВАЛСЯ СЛЕД АРГУМЕНТ ЗА ФЛАГОМ, ПОСМОТРЕТЬ С ПОМОЩЬЮ КАКОЙ ПЕРЕМЕННОЙ А НЕ ОПТИНД, ЧТОБЫ ЭТОТ АРГУМЕНТ 
//ДОБАВЛЯЛСЯ В МАССИВ ПАТЕРНОВ, А АРГУМЕНТ КОТОРЫЙ НЕ ФЛАГ И НЕ ШАБЛОН БУДЕТ ЯВЛЯТЬСЯ ФАЙЛОМ 

//ХОТЯ В ПАРСИНГЕ В ОПТАРГ ДОБАВЛЯЮТСЯ ШАБЛОНЫ И ФАЙЛЫ 

//не получается сделать поиск строки с пробелами непосредственно самим оригинальным grep -- а нужно ли делать экранирование с флагом е? -- мой код ловит все шаблоны а греп нет

//оригинальный греп ожидает после флага обязательно указания файла, нельзя написать: флаг-шаблон флаг-шаблон файл 

//можно: -флаг шаблон файл -флаг шаблон 
//можно: файл -флаг шаблон
//можно: файл -флаг шаблон -флаг шаблон
//можно: -флаг шаблон файл -флаг шаблон файл2
//можно: файл -флаг шаблон файл -флаг шаблон -- оба шаблона будут искаться в обоих файлах




//необычный вывод оригинального грепа в случае одного файла известного и одного неизвестного (пример в тг)
//в случае поиска в двух файлах греп выводит имя каждого файла и строки совпадений в этих файлах



//try to do some usual combinations: 
// grep pattern file
// grep -option pattern file 
// grep pattern file file2
// grep -option pattern file file2
// grep -opt -opt -opt pattern file


// grep -e pattern -f pattern.txt file.txt
// grep -i pattern file.txt -e pattern2 file2.txt

// т.е последним аргументом всегда является файл
// после флага только флаг или паттерн 
// если после флага был паттерн и не было флага далее только файлы
// только не с флагами е и f

int main(int argc, char *argv[]) {//
    char *input_files[argc];  
    char *pattern = "to del"; 
    flags fl = {0}; 
    
    fl.input_files = input_files;
   
    
    parser(&fl, argc, argv); 

    //кейс: когда нет флагов и может быть несколько файлов, когда несколько флагов подряд но не ф и е

    //я не понимаю что делать с ситуаций grep -opt patt -opt patt file --  считать как греп все после патерна файлом но не флаг и делать флаг
        if(!(fl.e||fl.f)){
            if(argc>2) {
                fl.patterns[fl.counter_patterns] = malloc((strlen(argv[optind])+1) * sizeof(char));
                strcpy(fl.patterns[fl.counter_patterns], argv[optind]); 
                fl.counter_patterns++;

                

                    for (int i = 0; i < fl.counter_patterns; i++)  
                        printf("fl.patterns: %d. %s\n", i, fl.patterns[i]);
                        //optind++; 

            } else {

                //мало аргументов
                //exit 

            }
        }

            for (int i = 0; i < argc; i++) {
                input_files[i] = NULL;
            }

            for (int i = optind, n = 0; i < argc; i++, n++) {
                input_files[n] = malloc((strlen(argv[i])+1) * sizeof(char));
                strcpy(input_files[n], argv[i]);
                fl.counter_files++;
                printf("input file %d: %s\n", fl.counter_files, input_files[n]);
            }
            //printf("input file %d: %s\n", i, input_files[i]);
   
    

        


    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
    
        process_file(argv[i], fl.patterns, fl); 

    }
    

    
    return 0;




    }


    /*if (argv[optind]) { 
        fl.patterns[fl.counter_patterns] = malloc((strlen(argv[optind])+1) * sizeof(char));
        strcpy(fl.patterns[fl.counter_patterns], argv[optind]); 
        fl.counter_patterns++;

    for (int i = 0; i < fl.counter_patterns; i++)  
        printf("fl.patterns: %d. %s\n", i, fl.patterns[i]);
        optind++; 
        */

    /*for (int i = 0; i < argc; i++) {
        input_files[i] = NULL;
    } */

    /*for (int i = 0; optind < argc; i++, optind++) {
        input_files[i] = malloc((strlen(argv[optind])+1) * sizeof(char));
        strcpy(input_files[i], argv[optind]);
        printf("input file %d: %s\n", i, input_files[i]);
    }*/
   /* for (int i = 0; input_files[i]; i++) {
        printf("input file %d: %s\n", i, input_files[i]);
    } */


    //printf("str16\n");
   // if (pattern == NULL) {
    //perror("Ошибка выделения памяти");
    //exit(EXIT_FAILURE);
//}
    
   




 void parser(flags *fl, int argc, char *argv[]){
        int opt = 0;
        int loginindex = 0;
        while(opt!= -1){
            opt = getopt_long(argc, argv, "e:f:ivclnhso", NULL, &loginindex); // : требует аргумента 
            printf("opt = %c\n", opt);
            switch(opt){
                
                case 'e':
                    printf("opt=%d optind=%d optarg=%s\n", opt, optind, optarg);
                    fl->e = 1;
                    fl->patterns[fl->counter_patterns] = malloc((strlen(optarg)+1) * sizeof(char));//+1 для хранения /0, чтобы функции не читали строку вне ее массива
                    if (fl->patterns[fl->counter_patterns] == NULL) {
                        //perror("Ошибка выделения памяти");
                        //exit(EXIT_FAILURE);
                    }
                    strcpy(fl->patterns[fl->counter_patterns], optarg);  
                    fl->counter_patterns++;
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
                    add_template_file(fl, optarg);
                break;
                case 'o':
                    fl->o = 1;
                    break;
                case '?':
                    printf("TODO copypast from grep error\n");
            }
        }
    }


void add_template_file(flags *fl, char *template_file) {
    FILE *file = fopen(template_file, "r");
    char *line = NULL;
    size_t length = 0; 

    if(!file){
        printf("ERROR\n");
        exit(1);
        // TODO 
        // grep: template1.txt: No such file or directory
        // exit from progranm
    }
    while ((getline(&line, &length, file)) != EOF) {
        printf("file line %s\n", line);
        fl->patterns[fl->counter_patterns] = malloc((length + 1) * sizeof(char));
        change_linebreak(line);
        strcpy(fl->patterns[fl->counter_patterns], line);
        fl->counter_patterns++;
    }
    fclose(file);
}

void process_file(char *filename, char **pattern, flags fl){
  
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t length = 0; 
    int match_count = 0; // for flag c
    int match_count_cv = 0;
    regex_t regex[10000]; 
    if(file == NULL){
        printf("error");
        exit(1);
    }
    compile_reg(regex, pattern, fl); //здесь компилируется шаблон поиска для регексес
    while( (getline(&line, &length, file)) != -1){ //читаем построчно
        change_linebreak(line); //заменяем ньюлайн на конец строки чтобы рег дальше работал
        int status = 1; //ноль если было совпадение 
        for (int i = 0; i<fl.counter_patterns; i++) {
            if(!regexec(&regex[i], line, 0, NULL, 0)) {
                status = 0;
            }
        }
        if(!status){
            match_count++;
        } else {
            match_count_cv++;
        }
        if(!status && !fl.c && !fl.v){
            printf("%s\n", line);
        } else if (status && fl.v && !fl.c){
            printf("%s\n", line);
        }
    }
    if(fl.c && !fl.v){
        c_flag(match_count);
    } else if (fl.c && fl.v){
        cv_flag(match_count_cv);
    }
    
        //regfree(regex);
        free(line);
        fclose(file);
     /*else if (!fl->s){ /// этот флаг для подавления сообщения об ошибке 
        printf("s21_grep: %s: No such file or directory\n", filename);
    }*/
}



void compile_reg(regex_t *regex, char **pattern, flags fl){
    for(int i = 0; i<fl.counter_patterns; i++){
        if(regcomp(&regex[i], pattern[i], fl.i ? REG_ICASE : REG_EXTENDED)){//если флаг i то выключаем учет регистра иначе расширенное регулярное выражение
            printf("PIZDA");
            exit(1);
        }
        
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


