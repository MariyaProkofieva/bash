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
                flags.v = 1;

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
       
        cat(flags, file); // ????? нужно ли передавать ch как укаазатель ????
    }

        fclose(file);

}



void cat(shortopts *flags, FILE *file){ 

    int ch; 
  
    int numb_line = 1; 
    int start_str = 1; 
    int prev = '\n';
    int new_line = 0;

        while( (ch = fgetc(file)) != EOF){ //пока символ это не конец файла
        

            if(prev == '\n' && ch == '\n'){
                
                new_line++;

            }

            
            if(flags->s && new_line>1) {
                while( ch == '\n'){
                    ch = fgetc(file);
                    
                }

                new_line = 0; 
            }

            if(start_str){

                if(flags->b){

                    if(ch != '\n'){
                        printf("%6d\t", numb_line); 
                        numb_line++;
                        start_str = 0;
                    }

                } else if(flags->n){

                    if(prev == '\n'){ 

                        printf("%6d\t", numb_line);
                        numb_line++;
                        start_str = 0;
                    }
                    
                }
            }
                if(flags->E){

                    if(ch == '\n'){
                        printf("$");
                    }

                    //ДОПИСАТЬ %
                }


                if(flags->v && ch != '\n' && ch != '\t'){
                        if(ch>127){
                            printf("M-");
                            ch -= 128;
                        }

                        if(ch == 127 || ch < 32){
                            printf("^");
                            if(ch == 127){
                                ch -= 63;
                            } else {
                                ch += 64;
                            }
                        }


                }

                if(flags->e && flags->v){

                    if(ch == '\n'){
                        printf("$");
                    }

                }

                if(flags->t && flags->v){
                    if(ch == '\t'){
                        printf("^");
                    }
                    

                }

                if(flags->T){ 
                    if(ch == '\t'){
                        printf("^");
                    }
                }
                
                putc(ch, stdout); 
                prev = ch;

                if(ch == '\n'){
                    start_str = 1;    
                }
                            
        }  

}


///маленькие флаги без v игнорируются 
   





   /*if(ch == '\n' && prev == '\n' ){
                        while(ch == '\n'){
                            ch = fgetc(file);
                        }
                        ungetc(ch, file);
                    }*/


                //ЧТО-ТО ПИСАТЬ В РИД МИ 