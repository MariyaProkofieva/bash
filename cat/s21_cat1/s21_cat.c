#include "s21_cat.h"

int main (int argc, char *argv[]){
   
    int opt = 0;
    int loginindex = 0;
    shortopts flags = {0};

    while(opt!=-1){//фун-я возвращает -1 в конце списка опций


        opt = getopt_long(argc, argv, "beEnstTv", longopts, &loginindex);//index нужен фун-и для обозначения длины длинного флага, он указывает на 
        //переменную хранящую индекс

            switch (opt) {
                //printf("str16\n");
                case 'b':
                flags.b = 1;
                break;
              
                case 'e':
                flags.e = 1; 
                flags.v = 1;
                
                case 'E':
                flags.e = 1; 
                break;
                
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
                flags.v = 1;

                break;
                case 'T':
                flags.t = 1;
                break;
                case '?':
                printf("s21_cat: illegal option -- %c", optopt);
                printf("usage: cat [-belnstuv] [file ...]");

            }

    }

  

    for(int i = optind; i < argc; i++){ //optind это переменная из getopt, указывает на следующий аргумент после флагов 
                //команда запуска нашего файла cat 
        open_close(argv[i], &flags); // происходит печать каждого файла командной строки
    }  

        
    
    return 0;
}



void open_close(char *filename, shortopts *flags) { // функция чтение и вывод файла
    FILE *file = fopen(filename, "rt"); //открытие текстового файла

    if (file != NULL){ // если файл открылся 
       
        cat(flags, file); 
    } else {
        printf("s21_cat: %s: No such file or directory\n", filename);
    }

        fclose(file);

}



void cat(shortopts *flags, FILE *file){ 

    int ch; 
    int numb_line = 1; 
    int start_str = 1; 
    int prev = '\n';
    int empty_line = 0;
    int s_start = 1;

        while( (ch = fgetc(file)) != EOF){ //пока символ это не конец файла

            if(s_start && ch == '\n'){       
                empty_line++;
            }else{
                empty_line = 0;
            }

            if(flags->s && empty_line>1) {
                while(ch == '\n' && ch!= EOF){
                    ch = fgetc(file);   
                } 
             
          //      empty_line = 0; 
            }

            if(start_str){
                
                if(flags->b){

                    if(ch != '\n'){
                        printf("%6d\t", numb_line); 
                        numb_line++;
                        start_str = 0;
                    }
                } else if(flags->n && ch != EOF){

                    if(prev == '\n'){ 

                        printf("%6d\t", numb_line);
                        numb_line++;
                        start_str = 0;
                    }
                    
                }
            }


                if( flags->v && ch != '\n' && ch != '\t' && ch != EOF){
                        if(ch>127){
                            printf("M-");
                            ch -= 128;
                        }

                        if(ch == 127 || ch < 32){
                            printf("^");
                            if(ch == 127){
                                ch -= 64;
                            } else {
                                ch += 64;
                            }
                        }

                } 
                
                
                if(flags->e && ch == '\n' && ch != EOF){
                    if(flags->b && prev == '\n' && ch != EOF){
                        printf("      \t$");
                    } else {
                        printf("$");
                    }
                }

                

                if(flags->t){
                    if(ch == '\t'){
                        printf("^");
                        ch += 64;
                    }  

                }
                
                if(ch!=EOF){
                    putc(ch, stdout); 
                }
                prev = ch;
       

                if(ch == '\n'){
                    start_str = 1; 
                    s_start = 1;   
                } else {
                    s_start = 0; 
                }
                            
        }

}  