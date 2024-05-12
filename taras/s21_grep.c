#include "s21_grep.h"

void parser(flags *fl, int argc, char *argv[]); 
void process_file(char *filename, char *pattern, flags *fl);
void compile_reg(regex_t *regex, char *pattern, flags *fl);
void change_linebreak(char *line);
void c_flag(int match_count);
void cv_flag(int match_count_cv);
void add_template(flags *fl, char *template_file);

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


int main(int argc, char *argv[]) {//
    char *input_files[argc]; //массив с указателями на отдельные строки имен файлов. каждый указатель ссылается на строку конкретного файла 
    // ??? почему аргс если файлов может быть меньше аргс или это типа макс допустимый
    char *pattern = "to del"; //шаблон 
    flags fl = {0}; //зануляем инты
    
    fl.input_files = input_files;//чтобы обращаться к структуре с **input_files, мы приравниваем структуру **input_files к *input_files 
    //т.е мы говорим на что ссылается **. указатели ** cсылаются на массив указателей *[ ], которые ссылаются на первые элементы строк 
    
    parser(&fl, argc, argv); //парсим
    // printf("optind=%d %s\n", optind, argv[optind]);
    if (argv[optind]) { //существует ли этот аргумент строки после парсинга, optind указывает на первый аргумент 
    //который не был обработан как флаг. т.е будет работать даже если флаг был в конце командной строки 
    //в случае отсутствия флагов optind = 1
        fl.patterns[fl.counter_patterns] = malloc((strlen(argv[optind])+1) * sizeof(char));//+1 для хранения /0, чтобы функции не читали строку вне ее массива
        //выделение памяти под шаблон с текущей длиной аргумента 
        //fl.counter_patterns = 0 изначально, мы занулили в начале структуру 
        //fl.patterns[fl.counter_patterns] - это конкретный указатель на строку шаблона
        strcpy(fl.patterns[fl.counter_patterns], argv[optind]); //копируем текущий аргумент в текущий шаблон 
        fl.counter_patterns++;//переходим на след массив-строку шаблона 
        
    for (int i = 0; i < fl.counter_patterns; i++)  // печатает все паттерны из fl.patterns в том числе и файл
        printf("fl.patterns: %d. %s\n", i, fl.patterns[i]);

    // записывает все аргументы правее паттерна в список входных файлов (может и не быть их)
    optind++; //переходим на новый аргумент 
    for (int i = 0; i < argc; i++) {
        input_files[i] = NULL;
    }

    for (int i = 0; optind < argc; i++, optind++) {
        input_files[i] = malloc((strlen(argv[optind])+1) * sizeof(char));//выделение памяти
        strcpy(input_files[i], argv[optind]);
        printf("input file %d: %s\n", i, input_files[i]);
    }
    for (int i = 0; input_files[i]; i++) {
        printf("input file %d: %s\n", i, input_files[i]);
    }


    //printf("str16\n");
    if (pattern == NULL) {
    //perror("Ошибка выделения памяти");
    //exit(EXIT_FAILURE);
}
    
   

    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
    
        process_file(argv[i], pattern, &fl); 

    }
    

    
    return 0;

}
}

 void parser(flags *fl, int argc, char *argv[]){
        int opt = 0;
        while(opt!= -1){
            opt = getopt(argc, argv, "e:f:ivclnhso"); // : требует аргумента 
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
                    add_template(fl, optarg);
                break;
                case 'o':
                    fl->o = 1;
                    break;
                case '?':
                    printf("TODO copypast from grep error\n");
            }
        }
    }


void add_template(flags *fl, char *template_file) {
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

void process_file(char *filename, char *pattern, flags *fl){
  
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t length = 0; 
    int match_count = 0; // for flag c
    int match_count_cv = 0;
    regex_t regex; 
    if(file != NULL){
        
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
    }
        regfree(&regex);
        fclose(file);
     /*else if (!fl->s){ /// этот флаг для подавления сообщения об ошибке 
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

// void f_flag(char *pattern, char *optarg) {//это для файла флага f, optarg - имя файла f
//     FILE *file = fopen(optarg, "r"); //открытие файла для шаблонов. optarg хранит аргумент флага
//     if(file){

//         char *line = NULL;
//         size_t length = 0; 
//         char *pattern_str;

//          while( (getline(&line, &length, file)) != -1){
//          pattern_str = malloc(strlen(line) + 1);
//          strcpy(pattern_str, line);
//          replace_linebreak(&copy_line);



//         }

//     } else {
//         printf("s21_grep: %s: No such file or directory\n", optarg);
//     }
// }













//относительно работаю флаги:
// i
// c
// c + i
// v
// v + c +i 
// начат s


