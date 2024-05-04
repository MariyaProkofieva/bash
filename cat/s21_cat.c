#include "s21_cat.h"

int main (int argc, char *argv[]){
   
    int opt, loginindex = 0;
    shortopts flags = {0};

    while(opt!=-1){//фун-я возвращает -1 в конце списка опций


        opt = getopt_long(argc, argv, "beEnstTv", longopts, &loginindex);//index нужен фун-и для обозначения длины длинного флага, он указывает на 
        //переменную хранящую индекс

            switch (opt) {
                //printf("str16\n");
                case 'b':
                flags.b = 1;
                break;
                case 'E':
                flags.E = 1;
                break;
                case 'e':
                flags.e = 1;
                flags.v = 1;

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

            //что если флаг введен некорректный 
            //если флага нет то не запускать cat и ввести в код переменную для отсутствия флага
    }

     //printf("%d\n", flags.b);


    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
                //команда запуска нашего файла cat 
        open_close(argv[i], &flags); // происходит печать каждого файла командной строки
    }  

        
    
    return 0;
}



void open_close(char *filename, shortopts *flags) { // функция чтение и вывод файла
    FILE *file = fopen(filename, "rt"); //открытие текстового файла

    if (file != NULL){ // если файл открылся 
        int ch = fgetc(file); // считывание первого символа ASCII в файле
        cat(flags, file, ch); // ????? нужно ли передавать ch как укаазатель ????
    }

        fclose(file);

}


void cat(shortopts *flags, FILE *file, int ch){ 

//вставить эту функцию после открытия файла
    
    int numb_line = 1; // для флага б и н
    int start_str = 1; 

    while(ch != EOF){ //пока символ это не конец файла

        if(flags->b){

            if(ch != '\n' && ch != EOF && start_str){
                printf("%6d\t", numb_line); 
                numb_line++;
                start_str = 0;
            }

        }

        if(flags->E){

            if(ch == '\n'){
                printf("$");
            }
        }

        if(flags->s){
            if(ch == '\n' && fgetc(file) == '\n' ){
                while(ch == '\n'){
                    ch = fgetc(file);
                }
            }
        }

        if(flags->n && !flags->b){

            if(start_str){ 

                printf("%6d\t", numb_line);
                numb_line++;
                start_str = 0;
            }
            
        }
        
        putc(ch, stdout); 
        ch = fgetc(file);

        if(ch == '\n'){
            start_str = 1;
        }
                    
    }
          

}

   





