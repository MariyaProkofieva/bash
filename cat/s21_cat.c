#include <s21_cat.h>

int main (int argc, char *argv[]){
    short_flags flags;

    for(int i = 1; i < argc; i++){ //начинаем с argv[1], тк в argv[0] находится
                                //команда запуска нашего файла cat 
        print_file(argv[i], &flags); // происходит печать каждого файла командной строки
    }  


    
    return 0;
}



void print_file(char *name, short_flags *flags) { // функция чтение и вывод файла
    FILE *file = fopen(name, "rt"); //открытие текстового файла

    if (file != NULL){ // если файл открылся 
        int ch = fgetc(file); // считывание первого символа ASCII в файле 
        while(ch != EOF){ //пока символ это не конец файла
            if(flags->e && ch=='\n'){
                putchar("$");
            } 
            putc(ch, stdout); //печатать символ
            ch = fgetc(file); // считываем новый символ и переписываем значение символа для печати  
        }
        fclose(file);
    }

}

void parser_flags(short_flags flags){ //необходим парсинг флагов, которые могут быть в комбинации и одному флагу 
// может соответствовать несколько вариаций символов 

// ??????? ДЛИННЫЕ ФЛАГИ

// тест-кейсы: vv / tt / TT / eV 



    while(){
            
            switch (... getopt_long(argc, argv, "beEnstTv", )!=-1) {
                case 'b':
                flags.b = 1;
                break;
                case 'E':
                flags.E = 1;
                break;
                case 'e':
                flags.e = 1;
                break;
                case 'v': 
                flags.v = 1;
                break;
                case 'n':
                flags.n = 1;
                break;
                case 's':
                flags.s = 1;
                break;
                case 't':
                flags.t = 1;
                break;
                case 'T':
                flags.T = 1;
                break;

            }
    }

        }

    }
}
