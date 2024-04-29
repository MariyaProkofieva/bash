#include s21_cat.h

int main (int argc, char *argv[]){

for(int i = 1; i < argc; i++){ //начинаем с argv[1], тк в argv[0] находится
                              //команда запуска нашего файла cat 
    print_file(argv[i]); // происходит печать каждого файла командной строки
}  


    
    return 0;
}



void print_file(char *name, fl) { // функция чтение и вывод файла
    FILE *file = fopen(name, "rt"); //открытие текстового файла
    if (file != NULL){ // если файл открылся 
        int ch = fgetc(file); // считывание первого символа ASCII в файле 
        while(ch != EOF){ //пока символ это не конец файла
            if(fl.e && ch=='\n'){
                putchar("$");
            } 
            putc(ch, stdout); //печатать символ
            сh = fgetc(f); // считываем новый символ и переписываем значение символа для печати  
        }

    fclose(file);

    }

}

void parser_flags(){ //необходим парсинг флагов, которые могут быть в комбинации и одному флагу 
// может соответствовать несколько вариаций символов 

// ??????? ДЛИННЫЕ ФЛАГИ

// тест-кейсы: vv / tt / TT / eV 

    for(int i = 1, j=0; i < argc, j < strlen(argv[i]); i++, j++){ // пройдемся по аргументам
        if( (argv[i][0]!= '-') || (strcmp(argc[i]; "-")) || strcmp(argc[i]; "--") ){
            break; //исключаем если первый элемент строки не -, если вся строка это -, если вся строка --
        } else {
            
            switch (argv[i][j]) {
                case 'b':
                fl.b = 1;
                break;
                case 'E':
                fl.E = 1;
                break;
                case 'e':
                fl.e = 1;
                break;
                case 'v': 
                fl.v = 1;
                break;
                case 'n':
                fl.n = 1;
                break;
                case 's':
                fl.s = 1;
                break;
                case 't':
                fl.t = 1;
                break;
                case 'T':
                fl.T = 1;
                break;

            }

        }

    }
}
