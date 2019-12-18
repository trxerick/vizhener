#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

void Decryptor(char *text , char* key, FILE *result_ptr ,int text_length); // Функция расшифровки текста

void Encryptor(char *text , char* key, FILE *result_ptr ,int text_length); // Функция шифровки текста

int Get_text_length(char* text); // Функция для поиска длины текста

char* Read_key_from_file(FILE* key_ptr, char* key, char* text ,int key_size , int text_length); // Функция для считывания ключа из файла

char* Read_text_from_file(FILE* text_ptr, char* text , int text_size); // Функция для считывания текста из файла

int main()
{
    FILE* key_ptr;
    FILE* text_ptr;
    FILE* result_ptr;
    char* key = NULL;
    char* text = NULL;
    int i = 0,key_length,key_size = 1,text_size = 1,text_length,choice;
    if((((key_ptr = fopen("key" , "r")) != NULL) && ((text_ptr = fopen("input" , "r")) != NULL) &&((result_ptr = fopen("output" , "w"))))){
       
        setlocale( LC_ALL,"Russian" );

        if((key =(char*)malloc(key_size * sizeof(char))) != NULL){   // Выделение памяти под ключ     
            if( (text = (char*)malloc(text_size * sizeof(char))) != NULL){ // Выделение памяти под текст

                text = Read_text_from_file(text_ptr , text, text_size);

                if(text != NULL){ // Если текст из файл считан полность, то продолжаем работать
                    text_length = Get_text_length(text); // Поиск длины считанного текста
                    key = Read_key_from_file(key_ptr , key , text ,key_size , text_length); // Считываем ключ из файла

                    if(key != NULL){
                        puts("Добро пожаловать в Vizhener`s! \n");
                        puts("---------------------------------------------------------------------------------");
                        puts("1 - Зашифровать текст из файла input.txt \n2 - Расшифровать текст из input.txt");
                        puts("---------------------------------------------------------------------------------");
                        scanf("%d" , &choice);
                        if(choice == 1){
                            Encryptor(text , key , result_ptr , text_length);
                        } else if(choice == 2){
                            Decryptor(text , key , result_ptr , text_length);
                        } else {
                            puts("Ошибка! Для выбора опции меню можно использовать только цифры 1 и 2");
                        }
                        free(text);
                        free(key);
                        text = NULL;
                        key = NULL;
                    } else {
                        puts("Произошла ошибка при выделении памяти для ключа!");
                        if(text != NULL){
                            free(text);
                            text = NULL;
                        }
                    }
                } else {
                    puts("Произошла ошибка при выделении памяти для текста");
                    if(key != NULL){
                        free(key);
                        key = NULL;
                    }
                }
        } else {
            puts("Произошла ошибка при попытке выделить память для текста!");
            if(key != NULL){
                free(key);
                key = NULL;
            }
        }
        } else {
            puts("Произошла ошибка при попытке выделить память для ключа!");
        }
                    puts("\nЛог программы:");

                    if(fclose(key_ptr) == EOF){
                            printf("Ошибка во время закрытия файла key.txt!");
                    } else {
                        printf("\nФайл key.txt успешно закрыт \n");
                    }
                    if(fclose(text_ptr) == EOF){
                        printf("Ошибка во время закрытия файла input.txt\n");
                    } else {
                                printf("Файл input.txt успешно закрыт!\n");
                    }
                    if(fclose(result_ptr) == EOF){
                        printf("Ошибка во время закрытия файла output.txt\n");
                    } else {
                        printf("Файл output.txt успешно закрыт!\n");
                    }
                    printf("\nНажмите ENTER, чтобы закончить работу программы");
                    getchar();
                    getchar();

    } else{
        puts("Произошла ошибка во время открытия файла!");
    }

    return 0;
}

void Decryptor(char *text , char* key, FILE *result_ptr ,int text_length)
{
    int i , j;
    char symb;
    puts("\n---------------------------------------------------------------------------------");
    puts("Вы выбрали опцию расшифровки текста \n");
        for(i = 0 , j = 0; i < text_length; i++ , j++){
            if(( (int)text[i] == 10) || ((int)text[i] == 32)){ // Если очередной символ является символом-перевода на новую строку
            // или пробелом, то просто записываем его файл без расшифровки
               j--;
               fputc(text[i],result_ptr);
            } else {
                symb = (((int)text[i] + 26 - (int)key[j]) % 26) + 65; // Расшифровываем символ и записываем его в файл
                fputc(symb,result_ptr);
            }
        }
        puts("УСПЕШНО РАСШИФРОВАНО! ПРОВЕРЬТЕ ФАЙЛ output.txt");
    puts("---------------------------------------------------------------------------------");
}

void Encryptor(char *text , char* key, FILE *result_ptr ,int text_length)
{
    int i , j;
    char symb;
    puts("---------------------------------------------------------------------------------");
    puts("Вы выбрали опцию зашифровки текста");
        for(i = 0 , j = 0; i < text_length; i++ , j++){
            if(( (int)text[i] == 10) || ((int)text[i] == 32)){// Если очередной символ является символом-перевода на новую строку
            // или пробелом, то просто записываем его файл без зашифровки
               j--;
               fputc(text[i],result_ptr);
            } else {
                symb = (((int)text[i] + (int)key[j]) % 26) + 65; // Зашифровываем символ и записываем его в файл
                fputc(symb,result_ptr);
            }
        }
        puts("УСПЕШНО ЗАШИФРОВАНО! ПРОВЕРЬТЕ ФАЙЛ output.txt");
        puts("---------------------------------------------------------------------------------");
}

int Get_text_length(char* text)
{
    int i = 0;
        while( ((int)text[i] >=65 && (int)text[i] <= 90) || ( (int)text[i] == 10 ) || ( (int)text[i] == 32 ) ){ // Подсчитываем
        // количество букв, пробелов и символов-перевода на новую строку в тексте
            i++;
        }
    return i;
}

char* Read_key_from_file(FILE* key_ptr, char* key, char* text ,int key_size , int text_length)
{
   int symb , flag = 1,key_length = 0 ,i = 0;

        while( ((symb = fgetc(key_ptr) ) != EOF ) && (flag != 0)){
            if(((int)symb >= 97) && ((int)symb <= 122)){
                            key[key_length] = (int)symb - 32; // Если буква в нижнем регистре, то переводим ее в верхний регистр
                            key_length++;
                        } else {
                            key[key_length] = symb;
                            key_length++;
                        }

            if(key_length == key_size){
                 key_size*=2;

                if((key = realloc(key ,key_size*sizeof(char))) == NULL){
                    flag = 0;
                 }
            }
        }

        if(flag == 1){
            key = realloc(key, text_length*sizeof(char));

            for(i = key_length; i < text_length; i++){
                key[i] = key[i-key_length];
            }
        }
    return key; 
}

char* Read_text_from_file(FILE* text_ptr, char* text , int text_size)
{
    int symb , flag = 1,text_length = 0, i =0,tmp = 0, count = 0;
    while( ((symb = fgetc(text_ptr) ) != EOF ) && (flag != 0)){ 
                   if(((isalpha(symb) != 0) || ((int)symb == 10)) || ((int)symb == 32)) {
                       count++;
                        if(text_length == text_size){
                            text_size*=2;
                            text = (char*)realloc(text ,text_size*sizeof(char));
                            if(text == NULL){
                                flag = 0;
                            }
                        }

                        if(flag != 0){
                            if(((int)symb >= 97) && ((int)symb <= 122)){
                                text[text_length] = (int)symb - 32;
                                text_length++;
                            } else {
                                text[text_length] = symb;
                                text_length++;
                            }
                        }

                    }
                }

            if(flag != 0){
                text = (char*)realloc(text,text_length*sizeof(char));
            }
        return text;
}
