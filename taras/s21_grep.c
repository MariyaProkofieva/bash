#include "s21_grep.h"

void parser(flags *fl, int argc, char *argv[]);
void process_file(char *filename, char **pattern, flags fl);
void grep_for_file(FILE *file, flags fl, char *filename, regex_t *regex);
int is_match(flags *fl, char *line, char *filename, regex_t *regex, int number_line);
void compile_reg(regex_t *regex, char **pattern, flags fl);
void change_linebreak(char *line);
void add_template_file(flags *fl, char *template_file);
void process_flag_c(flags *fl, int match_count, int dismatch_count, char *filename);
void process_line(flags *fl, char *filename, char *line, int match, int number_line);
void process_invalid_input(flags *fl);
void print_filename(flags *fl, char *filename);
void print_number(flags *fl, int number_line);
void add_newline_pattern(flags *fl, char *line, size_t length);
void parse_flag_e(flags *fl);


int main(int argc, char *argv[])
{ 
    char *input_files[argc];
    flags fl = {0};

    fl.input_files = input_files;

    parser(&fl, argc, argv);

    if (!(fl.e || fl.f))
    {
        if (argc > 2)
        {
            fl.patterns[fl.counter_patterns] = malloc((strlen(argv[optind]) + 1) * sizeof(char));
            strcpy(fl.patterns[fl.counter_patterns], argv[optind]);
            fl.counter_patterns++;
            optind++;
        }
        else
        {
            if (!fl.s) {
                printf("Missing arguments\n");
            }
            exit(1);
        }
    }

    for (int i = optind, n = 0; i < argc; i++, n++)
    {
        input_files[n] = malloc((strlen(argv[i]) + 1) * sizeof(char));
        strcpy(input_files[n], argv[i]);
        fl.counter_files++;
    }

    for (int i = optind; i < argc; i++)
    { 
        process_file(argv[i], fl.patterns, fl);
    }

    for(int i = 0; i<fl.counter_patterns; i++)
    {
        free(fl.patterns[i]);
    }

    for(int i = 0; i<fl.counter_files; i++)
    {
        free(fl.input_files[i]);
    }
    return 0;
}


void parser(flags *fl, int argc, char *argv[])
{
    int opt = 0;
    opterr = 0;
    int loginindex = 0;
    while (opt != -1)
    {
        opt = getopt_long(argc, argv, "e:f:ivclnhso", NULL, &loginindex); 
        switch (opt)
        {
        case 'e':
            parse_flag_e(fl);
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
            process_invalid_input(fl);
        }
    }
}

void parse_flag_e(flags *fl) {
    fl->e = 1;
    fl->patterns[fl->counter_patterns] = malloc((strlen(optarg) + 1) * sizeof(char)); 
    if (fl->patterns[fl->counter_patterns] == NULL)
    {
        exit(1);
    }
    strcpy(fl->patterns[fl->counter_patterns], optarg);
    fl->counter_patterns++;
}

void process_invalid_input(flags *fl) {
    if(!fl->s && optopt!='e'){
        printf("s21_grep: invalid option -- %c\n", optopt);
        printf("usage: s21_grep [-chilnsvo]\n");
        printf("[-e pattern] [-f file]\n");
    }

    if(!fl->s && optopt=='e'){
        printf("s21_grep: option requires an argument -- e\n");
        printf("usage: s21_grep [-chilnsvo]\n");
        printf("[-e pattern] [-f file]\n");
    }
    exit (1);
}

void add_template_file(flags *fl, char *template_file)
{
    FILE *file = fopen(template_file, "r");
    char *line = NULL;
    size_t length = 0;

    if (!file)
    {
        if(!fl->s){
            printf("s21_grep: %s: No such file or directory\n", template_file);
        }
        return;
    }
    while ((getline(&line, &length, file)) != EOF)
    {   
        if(*line == '\n'){
            add_newline_pattern(fl, line, length);
            continue;
        }
        fl->patterns[fl->counter_patterns] = malloc((length + 1) * sizeof(char));
        change_linebreak(line);
        strcpy(fl->patterns[fl->counter_patterns], line);
        fl->counter_patterns++;
    }
    free(line);
    fclose(file);
}

void process_file(char *filename, char **pattern, flags fl)
{
    FILE *file = fopen(filename, "r");
    regex_t regex[10000];
    if (file == NULL)
    {
        if(!fl.s){
            printf("s21_grep: %s: No such file or directory\n", filename);
        }
        return;
    }
    compile_reg(regex, pattern, fl);
    grep_for_file(file, fl, filename, regex);

    for (int i = 0; i < fl.counter_patterns; i++){
        regfree(&regex[i]);
    }
    fclose(file);
}

void grep_for_file(FILE *file, flags fl, char *filename, regex_t *regex) {
    char *line = NULL;
    size_t length = 0;
    int number_line = 0;
    int match_count = 0; 
    int dismatch_count = 0;
    int flag_match = 0;
    
    while ((getline(&line, &length, file)) != -1)
    {   
        number_line++;
        int match = is_match(&fl, line, filename, regex, number_line);
        change_linebreak(line);
        if(match){
            flag_match = 1;
            match_count++;
        } else {
            dismatch_count++;
        }
        if(!fl.c && !fl.o && !fl.l){
            process_line(&fl, filename, line, match, number_line);
        }
    }
    free(line);

    if(fl.c){
        process_flag_c(&fl, match_count, dismatch_count, filename);
    }

    if(fl.l && flag_match){
        printf("%s\n", filename);
    }
}

void compile_reg(regex_t *regex, char **pattern, flags fl)
{
    for (int i = 0; i < fl.counter_patterns; i++)
    {
        if (regcomp(&regex[i], pattern[i], fl.i ? REG_ICASE : REG_EXTENDED))
        { 
            exit(1);
        }
    }

}

void change_linebreak(char *line)
{
    int end_str = (int)strlen(line) - 1;
    if (line[end_str] == '\n')
    {
        line[end_str] = '\0';
    }
}



int is_match(flags *fl, char *line, char *filename, regex_t *regex, int number_line)
{
    int match = 0;
    regmatch_t regmatch; 
    for (int i = 0; i < fl->counter_patterns; i++)
    {
        if (!regexec(&regex[i], line, 1, &regmatch, 0)) {
            match = 1;
        }

        if (match && fl->o && !fl->c && !fl->v && !fl->l)
        {   
            char *cutted_line = line + regmatch.rm_eo;
            print_filename(fl, filename);
            print_number(fl, number_line);
            printf("%s\n", fl->patterns[i]);
            while (!regexec(&regex[i], cutted_line, 1, &regmatch, 0)) {
                cutted_line += regmatch.rm_eo;
                print_filename(fl, filename);
                print_number(fl, number_line);
                printf("%s\n", fl->patterns[i]);
            }
        }
       
    }
    return match;
}

void print_filename(flags *fl, char *filename)
{
    if (fl->counter_files > 1 && !fl->h)
    {
        printf("%s:", filename);
    }
}

void process_line(flags *fl, char *filename, char *line, int match, int number_line){
    if(!fl->v && match){
        print_filename(fl, filename);
        print_number(fl, number_line);
        printf("%s\n", line);
    } else if (fl->v && !match){
        print_filename(fl, filename);
        print_number(fl, number_line);
        printf("%s\n", line);
    }
}

void process_flag_c(flags *fl, int match_count, int dismatch_count, char *filename){
    print_filename(fl, filename);
    if (fl->l && match_count > 0) 
    {
        match_count = 1;
        printf("%d\n", match_count);
        return;
  }
    if(!fl->v){
        printf("%d\n", match_count);
    } else {
        printf("%d\n", dismatch_count);
    }

}


void print_number(flags *fl, int number_line){
    if(fl->n && !fl->c){
        printf("%d:", number_line);
    }
}

void add_newline_pattern(flags *fl, char *line, size_t length){
    fl->patterns[fl->counter_patterns] = malloc((length+1) * sizeof(char));
    strcpy(fl->patterns[fl->counter_patterns], line);
    fl->counter_patterns++;
}




