#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char *pattern = (char *)calloc(SIZE, sizeof(char)); //выделение памяти под массив символов
  //для хранения шаблона грепа 
  Flags flags = {false}; //инициализация всех флагов структуры нулями 
  Params params = {false, false, 0, 0}; //структура с ошибками и чем-то еще???

  if (argc < 2) { // не может быть меньше двух аргументов 
    fprintf(stderr, "Missing arguments\n"); 
    params.error = true; //ошибка 
  } else {
    check_flags(argc, argv, &flags, &params, &pattern);//парсинг флагов 
  }

  if (!params.error) {//если не ошибка 
    if (!flags.f && !flags.e) {//если не флаг f (для использования файла шаблонов) и одновременно не флаг е(для указания шаблона, полезно на случай 
    //экранированных символов 
      add_pattern(&pattern, argv[optind++]);
    }

    if (argc - optind > 1) params.multifile = true;

    for (int i = optind; i < argc; i++) {
      process_file(flags, &params, argv[i], pattern);
    }
  } else {
    fprintf(stderr, "Usage: ./s21_grep [flag] [pattern] [file]\n");
  }

  free(pattern);

  return params.error;
}

void check_flags(int argc, char *argv[], Flags *flags, Params *params,
                 char **pattern) {
  int option;

  while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && // : для аргументов требующих аргументы
         !(params->error)) { // params->error это структура для отслеживания ошибки, првоеряла  main кол-во аргументов и др
    if (option == 'e') {
      flags->e = true;
      add_pattern(pattern, optarg);
    } else if (option == 'i') {
      flags->i = true;
    } else if (option == 'v') {
      flags->v = true;
    } else if (option == 'c') {
      flags->c = true;
    } else if (option == 'l') {
      flags->l = true;
    } else if (option == 'n') {
      flags->n = true;
    } else if (option == 'h') {
      flags->h = true;
    } else if (option == 's') {
      flags->s = true;
    } else if (option == 'f') {
      flags->f = true;
      params->files_count++;
      process_file_pattern(params, pattern, optarg);
    } else if (option == 'o') {
      flags->o = true;
    } else {
      params->error = true; // ошибка отсутствия флагов? 
    }
  }

  if (flags->l) {
    flags->c = false, flags->n = false, flags->o = false; // ? противоречащие флаги
  }

  if (flags->c) {
    flags->n = false, flags->o = false; // ? противоречащие флаги 
  }

  if (optind >= argc || argv[optind][0] == '-' || strlen(argv[optind]) == 0) { // если индекс следующего аргумента больше или равен кол-ву аргументов
  //то достигнут конец строки, а обработано не все (БЛЯ А КАК?) типа имеются в виду флаги с :??? ИЛИ если первый аргумент равен просто - ИЛИ 
  //если аргумент пустой
    params->error = true; //то ошибка
  }
}

void process_file_pattern(Params *params, char **pattern, char *optarg) {//это для файла флага f, optarg - имя файла f
  FILE *file = fopen(optarg, "r"); //открытие файла для шаблонов 

  if (file != NULL) {//если существует 
    char *line = NULL; //строка из этого файла 
    size_t length = 0;//ее размер 

    while ((getline(&line, &length, file)) != -1) {//пока не конец файла считываем по строчке в line из файла 
      add_pattern(pattern, line); //добавляем считанную строчку в паттерн, то есть шаблон поиска 
    }

    free(line);//освобождение память порд строку 
    fclose(file);//закрываем файл шаблонов
  } else {
    params->error = true;//если файл не открылся то ошибка
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
  }
}

void add_pattern(char **pattern, char *line) {//перенос текста из файла f в шаблон поиска
  char *copy_line = malloc(strlen(line) + 10); // выделяем память под копию строки, +10 для запаса памяти на случай модификации строки
  strcpy(copy_line, line); //копируем line в copy_line 

  if (copy_line[0] == '\0' || copy_line[0] == '\n') { //если первый символ это знак конца строки или пустая, то добавляем знак конца строки??? зачем?
    strcpy(copy_line, "$");
  }

  replace_linebreak(&copy_line); // делает замену \n на спец символы чтобы работало регулярное выражение

  static size_t buffer = SIZE; // почему статическая? почему размер SIZE? что это?
  while (strlen(*pattern) + strlen(line) + 10 > buffer) { // проверка достаточно ли длины патерна для добавления новой строчки 
    buffer *= 2;// почему удваиваем? 
    *pattern = (char *)realloc(*pattern, buffer * sizeof(char));
  }

  sprintf(*pattern + strlen(*pattern), "%s\\(%s\\)", *pattern[0] ? "\\|" : "",
          copy_line);

  free(copy_line);
}

void replace_linebreak(char **line) {
  int last_index = (int)strlen(*line) - 1;
  if ((*line)[last_index] == '\n') (*line)[last_index] = '\0';
}

void process_file(Flags flags, Params *params, const char *filename,
                  char *pattern) {
  FILE *file = fopen(filename, "r");

  if (file != NULL) {
    regex_t regex;

    regex_compile(flags, params, &regex, pattern);

    if (!params->error && !(flags.o && flags.v)) {
      if (flags.o) {
        process_flag_o(flags, params, file, &regex, filename);
      } else {
        process_file_lines(flags, params, file, &regex, filename);
      }
    }

    regfree(&regex);
    fclose(file);
  } else if (!flags.s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
  }
}

void regex_compile(Flags flags, Params *params, regex_t *regex, char *pattern) {
  if (regcomp(regex, pattern, flags.i ? REG_ICASE : 0)) {
    params->error = true;
    fprintf(stderr, "Error compiling regex\n");
  }
}

void process_flag_o(Flags flags, Params *params, FILE *file, regex_t *regex,
                    const char *filename) {
  regmatch_t match;
  char *line = NULL;
  size_t length = 0;
  int line_count = 0, status;

  while ((getline(&line, &length, file)) != -1) {
    line_count++;

    replace_linebreak(&line);

    char *copy_line = strdup(line);
    char *original_copy_line = copy_line;

    while (((status = regexec(regex, copy_line, 1, &match, REG_NOTEOL)) == 0) &&
           strlen(copy_line)) {
      if (match.rm_so == match.rm_eo) {
        copy_line++;
        continue;
      }

      if (params->multifile && !flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", line_count);
      printf("%.*s\n", (int)match.rm_eo - (int)match.rm_so,
             copy_line + match.rm_so);

      copy_line += match.rm_eo;
    }

    free(original_copy_line);
    original_copy_line = NULL;
  }

  free(line);
  line = NULL;
}

void process_file_lines(Flags flags, Params *params, FILE *file, regex_t *regex,
                        const char *filename) {
  char *line = NULL;
  size_t length = 0;
  int line_count = 0, match_count = 0;

  while ((getline(&line, &length, file)) != -1) {
    line_count++;

    replace_linebreak(&line);

    int status = regexec(regex, line, 0, NULL, 0);

    if (!status ^ flags.v) {
      if (flags.l) {
        printf("%s\n", filename);
        break;
      }

      match_count++;

      if (!flags.c) {
        if (params->multifile && !flags.h) printf("%s:", filename);
        if (flags.n) printf("%d:", line_count);
        printf("%s\n", line);
      }
    }
  }

  if (flags.c) {
    if (params->multifile && !flags.h) printf("%s:", filename);
    printf("%d\n", match_count);
  }

  free(line);
  line = NULL;
}